# Axion™ - Discord Rooster Minigame Bot Template

![License](https://img.shields.io/badge/license-LGPLv3-blue)
![C++](https://img.shields.io/badge/language-C++17-green)
![Discord API](https://img.shields.io/badge/platform-Discord-blueviolet)

Axion™ is an open-source Discord bot template that turns your server into an arena of rooster champions!  
Build, train, and battle roosters with unique abilities and rarities — all through an interactive Discord interface.

---

## ✨ Features

- **Rooster Battles:** Turn-based fights with attacks, defense, healing, and special skills.
- **Collect Roosters:** Discover new roosters with unique stats, classes, and rarities.
- **Inventory System:** Manage your rooster and item collection.
- **Skills & Classes:** Roosters have classes (Tank, Healer, etc.) and unique abilities.
- **Interactive Gameplay:** Discord buttons and commands for smooth user experience.
- **Extensible:** Easily modify or expand for your own game ideas.

---

## 🕹️ Commands

- `/fight` — Start a rooster battle.
- `/inventory` — View your rooster and item collection.
- `/chest` — Open a chest for new roosters or items.
- `/skill` — Use a special skill in battle.
- `/stats` — Show your stats and leaderboard.
- `/help` — Command info.

Battle actions use Discord buttons: **Attack**, **Defend**, **Heal**, **Skill**, **Surrender**.

---

## 🧠 Tech Stack

- **Language:** C++17  
- **Framework:** [D++ (DPP)](https://github.com/brainboxdotcc/DPP)  
- **Database:** MongoDB (via mongo-cxx-driver)  
- **Build System:** CMake  
- **Platform:** Discord API  

---

## 🚀 Getting Started

Axion™ is a template, self-hosting is required.  
Clone and adapt for your own Discord bot projects!

### 🐧 Linux (Fedora example)

#### Dependencies

- CMake >= 3.10
- GCC/G++
- [mongo-cxx-driver](https://github.com/mongodb/mongo-cxx-driver)
- [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)
- [dpp](https://github.com/brainboxdotcc/DPP)

#### Install dependencies

```bash
sudo dnf install openssl-devel curl-devel cyrus-sasl-devel pkgconf-pkg-config
git clone https://github.com/mongodb/mongo-cxx-driver.git --branch releases/stable
cd mongo-cxx-driver/build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/local
make -j$(nproc)
sudo make install
sudo dnf install dpp-devel
```

#### Build Axion™

```bash
git clone https://github.com/apozinn/axion.git
cd axion
mkdir build && cd build
cmake ..
make -j$(nproc)
```

#### Run

```bash
export LD_LIBRARY_PATH=/usr/local/lib64
./axion
```

---

### 🪟 Windows

#### Dependencies

- [Visual Studio 2019/2022](https://visualstudio.microsoft.com/) (with C++ and CMake tools)
- [mongo-cxx-driver](https://github.com/mongodb/mongo-cxx-driver) (build with MSVC)
- [pkg-config for Windows](https://github.com/pkgconf/pkgconf)
- [dpp](https://github.com/brainboxdotcc/DPP)

#### Install dependencies

1. **Install Visual Studio** with Desktop C++ and CMake tools.  
2. **Install vcpkg:** [https://github.com/microsoft/vcpkg](https://github.com/microsoft/vcpkg)  
3. **Install mongo-cxx-driver and dpp via vcpkg:**
   ```powershell
   vcpkg install mongo-cxx-driver dpp
   ```
4. **Clone Axion:**
   ```powershell
   git clone https://github.com/apozinn/axion.git
   cd axion
   mkdir build && cd build
   ```
5. **Configure and build:**
   ```powershell
   cmake .. -DCMAKE_TOOLCHAIN_FILE=C:/path/to/vcpkg/scripts/buildsystems/vcpkg.cmake
   cmake --build . --config Release
   ```

#### Run

```powershell
axion.exe
```

---

## ⚙️ Configuration (`config.json`)

Before running Axion™, rename `config-example.json` to `config.json` and add your Discord bot token and MongoDB connection string:

```json
{
    "token": "YOUR_DISCORD_BOT_TOKEN",
    "mongo_db_uri": "YOUR_MONGODB_CONNECTION_STRING"
}
```

- **token:** Your Discord bot token. Get it from the [Discord Developer Portal](https://discord.com/developers/applications).  
- **mongo_db_uri:** Your MongoDB connection string. You can use [MongoDB Atlas](https://www.mongodb.com/atlas) or a local MongoDB instance.

> ⚠️ Never share your bot token or database credentials publicly!

---

## 🤝 Contributing

Contributions are welcome!  
Fork this repository and submit pull requests for features, bug fixes, or documentation improvements.

1. Fork & clone the repo  
2. Create a new branch (`git checkout -b feature/new-feature`)  
3. Commit and push changes  
4. Open a pull request  

---

## 📜 License & Credits

Axion™ is open-source and distributed under the [GNU Lesser General Public License v3.0 (LGPLv3)](https://www.gnu.org/licenses/lgpl-3.0.html).  
**If you use, fork, or modify this project, please credit the original author: [apozinn](https://github.com/apozinn).**

---

<p align="center"><b>⚔️ Build, train, and battle your roosters. Axion™ awaits! ⚔️</b></p>