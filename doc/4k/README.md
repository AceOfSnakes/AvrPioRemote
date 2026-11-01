# Update-Proof Environment Variable Guide (Linux & Windows)

This guide provides instructions to set application-specific environment variables that apply **only** to a specific program. These methods are completely update-proof: they pass along dynamic command-line arguments and will not be broken or overwritten when the application updates via a package manager (`.deb`, `.rpm`) or a Windows MSI installer.

---

## 🐧 Linux (KDE Plasma / Wayland / X11)

On Debian, Ubuntu, Kubuntu, or any apt-based system, we use a filesystem diversion (`dpkg-divert`). This tells the system's package manager: *"Whenever this app updates, move the incoming system binary out of the way, and leave my custom wrapper script untouched."*

### Step-by-Step Implementation

1. **Divert the original binary:**
   Tell the package manager to automatically rename the original binary to `.real` during installations or updates:
   ```bash
   sudo dpkg-divert --add --rename --divert /opt/avrpioremote-qt5/avrpioremote-qt5.real /opt/avrpioremote-qt5/avrpioremote-qt5
   ```

2. **Create the wrapper script:**
   Create a script at the original execution path so the system desktop shortcuts call it instead:
   ```bash
   sudo nano /opt/avrpioremote-qt5/avrpioremote-qt5
   ```

3. **Paste the wrapper logic:**
   Add the following content into the file. The `"$@"` sequence catches all arguments (existing or newly added by future updates) and passes them to the real app:
   ```bash
   #!/bin/sh
   export QT_ENABLE_HIGHDPI_SCALING=1
   export QT_SCALE_FACTOR=1.5
   export QT_AUTO_SCREEN_SCALE_FACTOR=0

   exec /opt/avrpioremote-qt5/avrpioremote-qt5.real "\$@"
   ```

4. **Make the wrapper script executable:**
   ```bash
   sudo chmod +x /opt/avrpioremote-qt5/avrpioremote-qt5
   ```

### To Revert Changes Later
If you ever want to uninstall the wrapper and restore default system package management behavior:
```bash
sudo rm /opt/avrpioremote-qt5/avrpioremote-qt5
sudo dpkg-divert --remove --rename /opt/avrpioremote-qt5/avrpioremote-qt5
```

---

## 🪟 Windows (MSI / EXE Installers)

TBD
