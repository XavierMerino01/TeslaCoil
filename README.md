# About
"Tesla Coil" is an engaging tower defense prototype where players defend a central Tesla tower located on the right side of the screen. Inspired by classic tower defense mechanics, the game combines strategic gameplay with dynamic action.

<div style="text-align: center;">
    <img src="https://github.com/XavierMerino01/TeslaCoil/assets/71768212/4bf09eac-2bf5-437f-acdf-2b16606cba05" alt="Captura" width="500">
</div>

# Gameplay
Players control a main Tesla tower, utilizing lightning strikes to fend off waves of enemies. Each shot depletes an energy bar that regenerates over time. Protect the tower's health from enemy attacks to survive through multiple challenging waves.

# Features

####  Tower Defense Dynamics
Defend against diverse enemy types with unique behaviors and attack patterns.
####  Progression System
Earn points to access a shop for upgrades like tower healing and increased maximum HP.
####  Structures 
Build and upgrade your factory and radio to deploy mini Tesla coils and unlock special abilities like airstrikes and helicopter support.
####  Objective
Survive all rounds of enemy waves to achieve victory.


<div style="display: flex;">
    <img src="https://github.com/XavierMerino01/TeslaCoil/assets/71768212/7a7e08a6-c3f3-4762-9055-f873e034c032" alt="Captura3" width="700">
</div>

# Development Focus
This project is part of my transition into C++ programming after having developed for some time in Unity. The goal was to integrate C++ with Unreal Engine's Blueprint visual scripting system, showcasing my skills in gameplay programming and design.

# Code Overview

#### Blueprint and C++ flow 
Established data exchange and functionality between C++ backend logic and Blueprint visual scripting. Leveraged Blueprint for most of the UI interactions, and visual effects, while using C++ for core game mechanics. This integration facilitated rapid iteration, debugging, and tuning of gameplay elements.

#### Inheritance for Systems and Enemies
Designed parent classes for "ADefenseTower" and "ABaseEnemy", using inheritance to handle diverse tower behaviors and enemy types effectively.

#### Modular Damage Handling
Created a HealthComponent to serve as a tool to handle damage and healing for the desired Actor, when attached. Aiming to implement diferent sources of damage towards the enemy (Main Tower, MiniCoil, Bomb...), and also keeping it's functionality for the player harming and healing.

#### Interactive Shop System
Developed a flexible shop (AGameShop) with a mapped function system to introduce and manage purchasable upgrades, facilitating the addition and testing of new shop functionalities when desired.
