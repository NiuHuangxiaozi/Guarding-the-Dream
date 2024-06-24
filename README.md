# <center>Guarding the Dream</center>



------



# Introduction:

This is a super game called  Guarding the Dream which is an interesting Tower Game like Carrot Fantasy.  Players should build the tower and defend the 

the Dream not be attacked. 



The outstanding feature of the game can be concluded as follows:

1.  🗺️Provide the map select feature. Players can build the game map as  they want. 

2.  👻We design normal monsters and some  special monsters  with some powerful skills.

3.  🗼 The game provides two types of tower which one is  short-range tower and  the other is  long-range tower.

4.  🌿Each tower can update and the max degree is three. With higher degree, the tower empowers higher damage , longer cast range and faster attack speed.

5.  💎We design  four special stones which can be mounted on the tower. The stones will enable the tower some unexpected  functions.

   

The Cover  of the game:

![startbackground](./ReadmePhoto/cover.png)



# Latest News：

- [2022/10] Finish the initial idea of the  game and  write  the code of the map function.
- [2022/11/01]  Finish the  tower function such as upgrade, attack range and build function.
- [2022/11/07] Finish some functions about monster such as movement and head. How to move along with the read and some interactions with the tower
- [2022/11/14] Finish the  special stones and the  special skills of the monster.
- [2022/11/28] Finish some bugs about background music and representation of the tower and monster.
- [2022/12/02] Fix some small bugs and beautify the UI.
- [2022/12/10] Finish all the function of the game.



# Details:

Here are some descriptions of this game:

#### Stage one : Map construction.

Users can build the map by themselves. The  game provide five different blocks which are shown as follows:

![startbackground](./ReadmePhoto/blocks.png)

Road Block: The leftmost block represent the road that monsters can walk through and you can also build short-range tower to prevent the enemy from advancing.

Decoration Block: The block with forests and  windmill just serves as a decoration of the map ( just for fun). 

Tower Platform Block: The white block showed here is a platform that users can build long-range towers here to attack the monsters . 

Birth Place and Target Place: The block which contains a red round represents  the birth place of the monsters  and the rightmost stands for the dream that you should defend and avoid the enemies to inter.



notice:

You should guarantee that the two points  (from the birth place to target place)  are connected by a path or the game will give out an error notice:

![startbackground](./ReadmePhoto/connect.png)



### Stage two : Ready to build your towers.

The picture below shows the key part of the game:

![mainwindow](D:\Tower\Tower\ReadmePhoto\mainwindow.png)



​		You can build short-range tower on the road block  and build long-range tower in the Tower platform block.  The picture shows four Birth place of the monster and one target place. 

​		The two types of points are connected by the road which are composed of road blocks.  We provide two types of towers. Each one has a cool down scroll.  

The details of the tower can be summed up as follows:

| Tower Type                                                 | Description                                                |
| ---------------------------------------------------------- | ---------------------------------------------------------- |
| ![army0](./ReadmePhoto/army0.gif) (level 1)                | the cheapest short-range tower :   ack=20 ; max_hp=1000    |
| ![armyone](./ReadmePhoto/armyone.gif)   (level 2)          | Base short-range tower :   ack=30 ; max_hp=1000            |
| ![armytwo](./ReadmePhoto/armytwo.gif) (level 3)            | Highest level short-range tower :   ack=40 ; max_hp=1000   |
| ![tower0](./ReadmePhoto/tower0.gif) （level 1)             | the cheapest long-range tower :   ack=10 ; attack-range=1  |
| ![magictower1](./ReadmePhoto/magictower1.gif)    (level 2) | Base long-range tower :   ack=15 ; attack-range=2          |
| ![magictower2](./ReadmePhoto/magictower2.gif)   (level 3)  | Highest level long-range tower :   ack=20 ; attack-range=3 |





​		We also provide four types of magic stones which will be described as follows:

| Stones                                                       | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| ![blood_select](./ReadmePhoto/blood_select.png)              | Poisonous Arrow:     Enemies hit by this special attack gradually lose health over time. |
| ![ice_select](./ReadmePhoto/ice_select.png)                  | Ice Sword:     Freeze enemies for several seconds.           |
| ![rampage_select](./ReadmePhoto/rampage_select.png)          | Rampage axe:     Increase the attack speed of the tower.     |
| ![group_injuries_select](./ReadmePhoto/group_injuries_select.png) | Area of effect Heart:     enable the tower can attack many enemies at the same time. |

Users can install two magic stones on the one tower which will  enable tower to  perform different effects. 

------



The GET MORE STARS button refers to update the tower.  User can use the score to build new tower and get scores through killing the monsters.





### Stage three: Combat and defend your dream.

Now  the game has four type monsters :

| Enemy Type                                                   | Description                                                  |
| ------------------------------------------------------------ | ------------------------------------------------------------ |
| ![enemyattack](./ReadmePhoto/enemyattack.gif)(normal)        | the normal monster which has no skills                       |
| ![enemydie](./ReadmePhoto/enemydie.gif)       (fast)         | the speed of the enemy is fast than others                   |
| ![enemyup](./ReadmePhoto/enemyup.gif)(flash)                 | This type of monster can skip one  short-range tower         |
| ![enemydieleft](./ReadmePhoto/enemydieleft.gif)(flash & fast) | This monster possesses the two powerful skills mentioned above. |





# Requirements

- qt-opensource-windows-x86-5.12.12  ,click here to  [download](https://www.qt.io/blog/qt-5.12.12-released).
- Windows 11 



# How to get:

Clone the repo and  open the tower.pro using Qt,  build the product and run the program.



# Fixed Issues:

if you  download the program and meet the question below,



# Videos:

- 
