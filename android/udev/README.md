# Install udev rules for OnePlus devices

First you will need to install *ADB* and *fastboot* binaries from your favorite distribution repository. Follow instructions from [prerequisite](#prerequisite) or go directly to the [install rules](#install-rules) section.

## [Prerequisite](#=prerequisite)

### [Install packages](#=install-packages)
On Ubuntu:
```
sudo apt-get install android-tools-adb android-tools-fastboot
```

On Fedora:
```
sudo dnf install android-tools
```

## [Install rules](#=install-rules)

Generate and install the rules for udev:
```
make && sudo make install
```

You may have to reload udev rules:
```
sudo make reload
```

```
make setup-adb
adb kill-server
adb shell
```

## [Setup user](#=setup-user)

To allow communication with your *Android* device without **root** privileges, you will create a *group* named **android** and be a part of that *group*. Run:
```
sudo make setup
```

Now you are supposed to run...
```
adb shell
```
.. to open a shell to your favorite *Android* device!
