# Supply Chain & Logistics Management System

A menu-driven supply chain logistics simulator built in C++ for our 2nd semester Object Oriented Programming course at FAST-NUCES Karachi.

## Features
- Create and manage shipment orders.
- Load cargo crates onto transport vehicles (trucks, planes, ships, drones).
- Dispatch shipments and track their status with calculated transit times.
- Run customs audits for international shipments (checks for hazmat and fragile rules).
- Save and load the system state to text files using custom File I/O.

## How It Works
The program runs in a command-line loop. You start by adding some vehicles to your fleet. Once you have a fleet, you can create cargo crates and assign them to shipment orders. 

When you try to load crates onto a vehicle, the system checks several rules. For example, drones can't carry over 50kg per crate, planes can't take hazmat materials, and ships can't take fragile items. If a rule is broken, it blocks the load. When you dispatch a shipment, it calculates the transit time based on the vehicle's speed and the distance. We also added a customs auditor for international orders that flags banned materials and calculates tariffs. You can save all your active data to text files and load it back up later.

## OOP Concepts Used
- **Inheritance:** `GroundTransport`, `AirTransport`, and `WaterTransport` inherit from an abstract `TransportAsset` base class. We also used multiple virtual inheritance for the `HeavyLiftDrone` so it can act as both air and ground transport without hitting the diamond problem.
- **Polymorphism:** We used pure virtual functions (like `calculateTransitTime()` and `loadCrate()`) so each vehicle type runs its own specific logic.
- **Templates:** Instead of relying on `<vector>`, we built a custom `Registry<T>` template class to manage our dynamic arrays of pointers.
- **Exception Handling:** We made a custom exception hierarchy starting with `LogisticsException` to handle things like exceeding payload capacity or violating cargo rules.
- **Operator Overloading:** We overloaded `operator+` to handle capacity checks when loading crates, and `operator[]` for easy array indexing.
- **Friend Classes:** The `CustomsAuditor` is a friend class to `ShipmentOrder`, which lets it check the private crate arrays directly without opening up security holes with public setters.

### How to Run
1. **Compile:** Open your terminal (VS Code Terminal, Command Prompt, or PowerShell) and run: `g++ Main.cpp Assets.cpp Logistics.cpp -o Logistics`
2. **Run on Windows (Default):** In that same terminal, execute the program by typing `Logistics.exe` (if using Command Prompt) or `.\Logistics.exe` (if using PowerShell or VS Code).
3. **Run on Linux/macOS:** Before compiling, open `Main.cpp` and replace `system("cls")` with `system("clear")`. Then compile and run using `./Logistics`.

## Made by
[@MF4rhan](https://github.com/MF4rhan) & [@farazcmd](https://github.com/farazcmd)
