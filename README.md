# Turn-based Strategy Game
A turn-based dungeon crawling roguelike game built in C++ using SFML.  
Explore dungeons, battle enemies, collect skills, and survive!

## Features
- Tile-based dungeon exploration with grid movement  
- Turn-based combat system with player and enemy turns  
- Skills and status effects including stun and poison  
- Level up, gain experience, and collect new abilities  
- Boss fights and multi-room dungeons  
- Save and load your progress (player stats, enemies, map orbs)  
- Popup battle result UI showing victory/defeat, EXP gain, and new skills
- Enemies with different traits (difficulty, resistance, etc)
- Orbs and doors available for interaction
- Inventory for managing collected skills

## Installation

### Prerequisites
- C++17 compiler  
- [SFML 2.5+](https://www.sfml-dev.org/download.php) installed  
- make build tool

### Build 
1. Clone the repo and checkout your working branch
git clone --branch master https://github.com/delicruz/OOP-Projecct.git
cd OOP-Projecct
2. Build with Makefile
make

## Usage
Run the compiled executable file:
bin/turn_based_game

Use arrow keys to move your player around the dungeon. Interact with doors and healing orbs with the spacebar.

## Controls
- Arrow Keys: Move player, selecting options
- Space: Interact with orbs, doors
- I: open Inventory, close Inventory
- Esc: close Inventory
- R: exit the battle (after enter the door)
- F: enter the battle (after enter the door
- Enter: equipped skills, using skills in battle

## Gameplay
- Explore the dungeon map and enter battles through doors
- Use physical attacks, magic, or skills in turn-based combat  
- Enemy resistance halves incoming damage **based on attack type**, applied before damage is dealt  
- Status effects like stun cause characters to skip their next turn, poison cause a certain amount of damage through certain rounds
- Defeating enemies help player to gain exp and new skill
- When player is defeated, they will respawn with full hp
- Gain exp to reach a certain level to spawn the Boss
- Save/load your game progress at anytime
- Defeat Boss to win the game
