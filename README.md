# SaveLoad
O3DE Save Load Gem licensed under the [Expat license](/LICENSE.txt).

## Installation Instructions
You can install this gem in your project using O3DE's project manager. Navigate to the Gem Repositories page and provide this repo's git URL.

For more information, see the instructions found in O3DE's documentation: https://www.o3de.org/docs/user-guide/gems/repositories/overview/#add-a-gem-repository-in-project-manager

## How To Use This Gem
Since this gem uses the SaveData gem, it is not meant to be used in the O3DE editor, but rather with your exported <game-name>.GameLauncher game distribution binary.

So to use this gem, you must export your game as a distributable binary by running the `export.sh` or `export.bat` script within your project directory, and then running your game using either `<project-directory>/build/export/<project-name>GamePackage/<project-name>.GameLauncher` or `<project-directory>/build/launcher/bin/release/<project-name>.GameLauncher`.

Whenever you perform a save of either buffer data or object parameters, you will find the save files at the following locations in your filesystem:

**Linux:**

* `~/.local/share/<project-name>.GameLauncher/SaveData/<buffer-data-file-name>.savedata` (this is a plain text file)
* `~/.local/share/<project-name>.GameLauncher/SaveData/<object-parameters-file-name>.savedata` (this is a binary file)

**Windows:**

* Same files as Linux except they could be located in any of the following directories (retrieved using the Windows SHGetKnownFolderPath function):
  * `C:\Users\{username}\AppData\Local`
  * `C:\Users\{username}\AppData\Roaming`
  * `C:\Users\{username}\Documents`
  * `C:\Users\{username}\Saved Games`

## License
This project's source and header files are licensed under the [Expat license](/LICENSE.txt).
