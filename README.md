# Box Shooter : FPS Challenge - Unreal Engine 5.6
---

## Overview

This project is a first-person shooter (FPS) game developed in **Unreal Engine 5.X**. The core gameplay revolves around dynamically spawning interactive boxes based on real-time data fetched from an online JSON endpoint. Players engage by shooting and damaging these boxes, earning points upon destruction, which are displayed on a custom in-game UI.

This assignment focuses on demonstrating **Unreal Engine C++ proficiency** in areas such as online data fetching, JSON parsing, dynamic actor spawning, custom material application, hit detection, and UMG UI integration.


## Features

- Spawns Boxes from Online Data: The game connects to the internet to get details (like color, health, and points) for the boxes it needs to create.
- Boxes to Shoot: The boxes appear in the game. You can shoot them to reduce their health. A health widget is displayed to show their health.
- Changes Color: The boxes have different colors based on the information from the internet.
- Score Points: When a box's health reaches zero, it breaks, and you get points.
- Shows Your Score: Current score is always visible on the screen.

---

## Technical Logic & Implementation Notes

### Data Fetching & Box Spawning
- **HTTP Requests:** Utilizes Unreal Engine's native HTTP Module to get requests to retrieve the JSON data from the URL.
- **JSON Parsing:** Parse the incoming JSON string into Unreal data types. This data then sets the properties of each spawned box.
* **Box Actor:** Upon parsing, instances of this actor are spawned with their properties (type, color, health, score) initialized from the JSON data.

### Interaction & UI
- **Shooting:** using Unreal’s projectile movement component to shoot the boxes
- **Damage & Score:** When a projectile hits a box, its health is decremented. If health reaches zero, the box is destroyed, and the player's score is updated.
- **Score UI:** A Widget manages the score display, updating on screen whenever the player's score changes.

---

## Challenges Faced

The primary challenge encountered during development was related to **fetching and parsing the online JSON data**:

* **Asynchronous HTTP Operations:** Handling the asynchronous nature of web requests required careful implementation of delegates to ensure that data was processed only after a successful response, avoiding race conditions or null data access.

* **Robust JSON Parsing:** Ensuring the parsing logic could gracefully handle variations or potential malformations in the JSON structure (e.g., missing fields, incorrect data types) was crucial for stability. This was addressed by consistently using `TryGet...` methods provided by `FJsonObject` and `FJsonValue` and implementing appropriate error logging. This ensured the application wouldn't crash if the external data source was not perfectly structured.

---

## 🎮 How to Test the Feature

1.  **Clone the Repository:**
    ```bash
    git clone [https://github.com/YourGitHubUsername/BoxShooter.git](https://github.com/YourGitHubUsername/BoxShooter.git)
    ```
2.  **Navigate to Project Directory:**
    ```bash
    cd BoxShooter
    ```
3.  **Generate Project Files:**
    * Right-click on `BoxShooter.uproject` and select "Generate Visual Studio project files".
4.  **Open & Build in Unreal Engine:**
    * Open `BoxShooter.uproject` directly, or open the generated `BoxShooter.sln` (or equivalent) in your IDE, then build the "Development Editor" configuration.
5.  **Run the Game:**
    * In the Unreal Editor, press the **"Play"** button to start the game in the viewport.


