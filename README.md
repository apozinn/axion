# Axion™ - Discord Rooster Minigame Bot Template

<p align="center">
  <a href="https://github.com/apozinn/axion/blob/main/LICENSE" target="_blank">
    <img alt="License" src="https://img.shields.io/badge/license-LGPLv3-0078D6?style=for-the-badge&logo=gnu&logoColor=white">
  </a>
  <a href="https://github.com/apozinn/axion" target="_blank">
    <img alt="Language" src="https://img.shields.io/badge/C%2B%2B-17-00599C?style=for-the-badge&logo=c%2B%2B&logoColor=white">
  </a>
  <a href="https://dpp.dev/" target="_blank">
    <img alt="Framework" src="https://img.shields.io/badge/D%2B%2B%20(DPP)-blueviolet?style=for-the-badge&logo=discord&logoColor=white">
  </a>
</p>


**Axion™** is an **open-source Discord bot template** that turns your server into an arena of rooster champions!

Build, train, and battle roosters with unique abilities and rarities — all through a modern, interactive Discord interface. It serves as a robust starting point for any developer looking to create their own complex RPG-style minigame or persistent world within Discord.

---

## ✨ Key Features

| Feature | Description |
| :--- | :--- |
| **Strategic Battles** | Turn-based combat featuring attacks, defense, healing, and special skills. |
| **Rooster Collection** | Discover and collect roosters with unique stats, classes, and rarity levels. |
| **Classes & Skills** | Roosters have distinct classes (Tank, Healer, Damage) and customizable ability sets. |
| **Modern UI** | A fluid, interactive user experience using **slash commands** and **Discord buttons** for battle actions. |
| **Extensible** | Clean **C++17** architecture, making it easy to modify and expand for your own game ideas. |

---

## 🕹️ Commands & Gameplay

The game focuses on a seamless experience, with buttons driving battle actions.

| Command | Description |
| :--- | :--- |
| **`/fight`** | Starts a rooster battle by challenging another player. |
| **`/inventory`** | Views your current collection of roosters and items. |
| **`/stats`** | Shows your personal statistics and the global leaderboard. |
| **`/skills`** | Displays all available skills in the game. |
| **`/help`** | Displays detailed command information. |

**Battle Actions (Buttons):** **Attack**, **Defend**, **Heal**, **Skill**, **Surrender**.

---

## 🧠 Tech Stack

Axion™ is built for high performance and scalability, leveraging the following technologies:

- **Language:** **C++17** (Modern C++ Standard)
- **Framework:** **[D++ (DPP)](https://github.com/brainboxdotcc/DPP)** — A fast and powerful library for the Discord API.
- **Database:** **MongoDB** (via mongo-cxx-driver) — Ideal for flexible game data.
- **Build System:** **CMake**
- **Platform:** **Discord API**

---

Essa informação de instalação já estava presente na sua versão original do README. Eu a reorganizei e incorporei no novo layout simplificado, garantindo que as instruções para **Linux** e **Windows** estejam completas e claras.

Aqui está a seção aprimorada **"Setup & Getting Started"** (Configuração e Primeiros Passos) com as instruções de instalação do `mongo-cxx-driver` e `DPP` detalhadas:

## 🚀 Setup & Getting Started

Axion™ is a **template**, and **self-hosting is required**. Follow the steps below to clone and configure your own bot.

### ⚙️ 1. Essential Configuration (`config.json`)

Before running, rename `config-example.json` to **`config.json`** and populate it with your credentials:

```json
{
    "token": "YOUR_DISCORD_BOT_TOKEN",
    "mongo_db_uri": "YOUR_MONGODB_CONNECTION_STRING",
    "language": "en_US"
}
```

  * **Token:** Get this from the [Discord Developer Portal](https://discord.com/developers/applications).
  * **MongoDB URI:** You can use [MongoDB Atlas](https://www.mongodb.com/atlas) or a local MongoDB instance.

> ⚠️ **Security Warning:** Never share your bot token or database credentials publicly\!

### 💻 2. Build and Run

Axion™ requires **CMake \>= 3.16** and the **mongo-cxx-driver** installed on your system.

#### 🐧 Linux (Manual Installation Example)

This example shows the manual steps to install the necessary libraries (`mongo-cxx-driver` and `DPP`) before building Axion™.

##### Install `mongo-cxx-driver` and `DPP`

1.  **Install system dependencies (e.g., Fedora/dnf):**
    ```bash
    sudo dnf install openssl-devel curl-devel cyrus-sasl-devel pkgconf-pkg-config
    ```
2.  **Build and install `mongo-cxx-driver`:**
    ```bash
    git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/stable 
    cd mongo-cxx-driver/build 
    cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local 
    make -j$(nproc) 
    sudo make install
    ```
3.  **Install DPP:** Follow the specific instructions for your distribution on the [DPP Installation Guide](https://dpp.dev/installing.html).

##### Build Axion™

```bash
git clone https://github.com/apozinn/axion.git 
cd axion
mkdir build && cd build 
cmake .. 
make -j$(nproc)
```

##### Run

```bash
export LD_LIBRARY_PATH=/usr/local/lib64
./axion
```

#### 🪟 Windows (Recommended: vcpkg)

Using **vcpkg** is the simplest way to manage the C++ dependencies for Windows.

1.  **Prerequisites:** Install [Visual Studio 2019/2022](https://visualstudio.microsoft.com/) (with C++ and CMake tools) and [vcpkg](https://github.com/microsoft/vcpkg).
2.  **Install Libraries (`mongo-cxx-driver` and `DPP`):**
    ```powershell
    vcpkg install mongo-cxx-driver dpp
    ```
3.  **Clone and Configure:**
    ```powershell
    git clone https://github.com/apozinn/axion.git
    cd axion/
    mkdir build && cd build
    cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
    ```
4.  **Build and Run:**
    ```powershell
    cmake --build . --config Release
    .\Release\axion.exe
    ```

## 🤝 Contributing

Contributions are highly welcome\! Whether it's bug fixes, new features, or documentation improvements.

1.  **Fork** and clone the repository.
2.  Create a new branch (`git checkout -b feature/minigame-upgrade`).
3.  Commit and push your changes.
4.  Open a **Pull Request**.

-----

## 📜 License & Credits

Axion™ is an open-source project distributed under the **[GNU Lesser General Public License v3.0 (LGPLv3)](https://www.gnu.org/licenses/lgpl-3.0.html)**.

**If you use, fork, or modify this project, please credit the original author: [apozinn](https://github.com/apozinn).**

-----

<p align="center"><b>⚔️ Build, train, and battle your roosters. The Axion™ arena awaits! ⚔️</b></p>
