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

Windows does not have `dpkg-divert`, and MSI installers frequently overwrite or replace Desktop/Start Menu shortcuts during upgrades. To solve this, we use a built-in kernel feature called **Image File Execution Options (IFEO)**. Whenever `avrpioremote-qt5.exe` is called, Windows intercepts it and routes execution through a persistent script.

### Step-by-Step Implementation

1. **Create the VBScript Wrapper:**
   Using a VBScript ensures your application launches directly without flashing a brief, blank black command prompt window. Open **Notepad** and paste:
   ```vbs
   Set objShell = CreateObject("WScript.Shell")
   Set objEnv = objShell.Environment("PROCESS")

   ' 1. Set environment variables
   objEnv("QT_ENABLE_HIGHDPI_SCALING") = "1"
   objEnv("QT_SCALE_FACTOR") = "1.5"
   objEnv("QT_AUTO_SCREEN_SCALE_FACTOR") = "0"

   ' 2. Collect all command line arguments passed by the shortcut dynamically
   strArgs = ""
   For Each arg In WScript.Arguments
       strArgs = strArgs & " """ & arg & """"
   Next

   ' 3. Run the target executable directly
   objShell.Run """C:\Program Files\avrpioremote-qt5\avrpioremote-qt5.exe""" & strArgs, 1, False
   ```
   *Note: Ensure the absolute path in line 3 matches your exact `.exe` directory location.*

2. **Save the script:**
   Save this file inside the application directory as `qt5-scaling-wrapper.vbs`. Make sure to set "Save as type" to **All Files (*.*)** so Notepad does not append `.txt`.
   * Target Path Example: `C:\Program Files\avrpioremote-qt5\qt5-scaling-wrapper.vbs`

3. **Register the interceptor in the Registry:**
   Open a new blank **Notepad** window and paste the following text to create a Registry modification file:
   ```registry
   Windows Registry Editor Version 5.00

   [HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\avrpioremote-qt5.exe]
   "Debugger"="wscript.exe \"C:\\Program Files\\avrpioremote-qt5\\qt5-scaling-wrapper.vbs\""
   ```

4. **Apply the Registry configurations:**
   Save this file to your Desktop as `override.reg` (selecting **All Files (*.*)**). Double-click `override.reg` and accept the administrator UAC prompts to merge the keys into your system configuration.

### To Revert Changes Later
If you want to stop the redirection, open the Windows Registry Editor (`regedit`) and delete the `avrpioremote-qt5.exe` key from the following folder:
`HKEY_LOCAL_MACHINE\SOFTWARE\Microsoft\Windows NT\CurrentVersion\Image File Execution Options\`
