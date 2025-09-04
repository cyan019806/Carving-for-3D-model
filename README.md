# Carving-for-3D-model

實作有雕刻效果的三維體素模型

包含以下幾個部分:
1. 體素化
   使用體素模型替代傳統的三角片模型
   優點: 1. 可以建構較複雜的模型 2. 不只能表現模型表面，甚至包含其內部結構
   像素(pixel): 在二維空間中儲存資料的最小單位 
   體素(voxel): 在三維空間中儲存資料的最小單位
   <img width="491" height="276" alt="image" src="https://github.com/user-attachments/assets/a1af6fcf-10ae-485a-8ad5-ad1ded8bff18" />
   
   左圖為單位體素 右圖為體素模型
2. 距離場計算
   距離場: 由空間中每一個點到模型表面的最短距離所形成的場
   1) 計算距離場實現模型分層
      透過計算距離場賦予每個體素距離值得以實現模型分層
      計算依據以下偏微分方程式，描述空間中任一點 (x, y, z) 到模型表面(距離值為0)的距離:
      
      <img width="300" height="86" alt="image" src="https://github.com/user-attachments/assets/a1e30034-494e-4700-842e-05520a7ba7d7" />
      D 代表空間中位置 (x,y,z) 到模型表面的距離值
      式中的<img width="130" height="52" alt="image" src="https://github.com/user-attachments/assets/38e68458-ac71-4e71-9732-af89f8685389" />
      代表每個體素在 x,y,z 三個方向中的梯度方向(即距離值是從哪傳遞過來)
      梯度方向為距離場擴散最快的方向，因此可作為後續的雕刻方向
      
   3) 區分模型內外
      將內層距離值設為正值，外層距離值設為負值
      雕刻為由模型表面向外或向內進行，因此需要將模型區分內外部
      將三維空間切成一層層的二維平面，舉例而言為xy平面，並且空間大小為 NxM
      從 y = 0 到 y = M-1 沿著 +x 方向發射射線，在遇到表面體素之前的體素皆為外部體素，距離值乘上負號。一旦遇到表面體素後則需判斷掃描過的鄰居體素是否為外部體素，是的話為外部體素；否的話為內部體素。
      由於這樣判斷對某部分模型會無法正確判斷，因此需要再從 y = M-1 掃描到 y = 0，用一樣的判斷方法進行判斷
      yz 平面及 xz 平面也透過一樣的方式進行判斷，只要這六個方向中有三次以上被判斷為外部體素即為外部體素，否則為內部體素
      
3. SOM 訓練
   無法直接將 2D 的 texture 與 3D 的 模型直接進行對應，因此需要透過 SOM 來作為媒介
   第一階段先訓練 3D 的空間座標，產生貼合模型表面的擬合曲面
   <img width="1171" height="386" alt="image" src="https://github.com/user-attachments/assets/a0d56cd7-4646-40ff-9ff5-0ec5d5861ec3" />

   第二階段再訓練 2D 的材質座標，使其參數化
   <img width="1126" height="427" alt="image" src="https://github.com/user-attachments/assets/a8ce1e54-c6af-4eb5-8c17-90d0d4715115" />

5. 紋理映射
   分為以下兩階段
   1) 體素與神經元對應
      由於體素的數量遠大於神經元，因此需要進行 濾波(Filtering)
      找到與體素的空間座標最接近的神經元 BMU
      根據 BMU 在神經網路的位置會與附近的神經元形成1、2或4個三角形
      將體素對這些三角形做投影
      再用計算重心座標的方式計算體素對應到的神經元
      
   2) 神經元與材質貼圖對應
      將神經元與材質貼圖做對應找到顏色值
      根據 texel resolution 與 voxel resolution 大小
       1. texel resolution < voxel resolution => Magnification
          將鄰近像素點的顏色值做雙線性內插
       2. texel resolution >= voxel resolution => Minification
          用盒式濾波的方式將貼圖範圍內的顏色值相加做平均
          
6. 雕刻
   根據每個體素對應的顏色資訊決定是否移除或保留
   常見的雕刻方式有以下三種:
   1) 陰刻
      移除圖案本身，形成表面內凹的模型
      <img width="1193" height="332" alt="image" src="https://github.com/user-attachments/assets/d8daf047-f750-40a0-8653-a90317563368" />

   2) 陽刻
      移除圖案以外部分，形成表面外凸的模型
      <img width="1205" height="323" alt="image" src="https://github.com/user-attachments/assets/5896aece-d57f-4547-8e0d-936c1e278025" />
   
   3) 鏤空
      在陽刻基礎上移除模型內層，形成穿透的模型
      <img width="1186" height="328" alt="image" src="https://github.com/user-attachments/assets/e65c7492-aa74-424a-b8f1-4adafccee6ec" />
      
7. 形態學運算
   使用開啟運算或閉合運算來優化結果
   1) 陰刻後使用閉合運算
      <img width="1120" height="437" alt="image" src="https://github.com/user-attachments/assets/ea12e6b7-e9c7-4e81-845b-481fce1aef1a" />

      左圖為無使用閉合運算，右圖為使用閉合運算
          
   2) 陽刻及鏤空後使用開啟運算
      <img width="1132" height="438" alt="image" src="https://github.com/user-attachments/assets/87036c5a-e089-4b20-83de-7c01d11019f4" />

      左圖為無使用開啟運算，右圖為使用開啟運算  
