# CPP library to make BLE GATT server using BLUEZ DBus API
This library provides a easy way to make BLE GATT server, but do not provide bluetooth managment API
[bluetooth managment API](https://git.kernel.org/pub/scm/bluetooth/bluez.git/tree/doc/mgmt-api.txt)

### Implemented features
- GATT services
- GATT characteristics
    - Read
    - Write
    - Write Without response
    - Notify
- Device Property Changed events
    - Address
    - AddressType
    - Name
    - Alias
    - Paired
    - Trusted
    - Blocked
    - LegacyPairing
    - Connected
    - Adapter
    - ServicesResolved
    - RSSI

### How to start
1.  Write a code
    ```cpp
    #include "dbus_gatt/dbus_gatt.h"

    class YourGATTAppName : public dbus_gatt::DBusGATT {
        YourGATTAppName(): dbus_gatt::DBusGATT("your-gatt-app-bus-name", "/your/gatt/app/bus/path") {
            Services(dbus_gatt::DBusGATT::Service(
                "your-gatt-app-service", // service name
                "00000001-0000-0000-0000-000000000001", // service uuid
                dbus_gatt::DBusGATT::ReadOnlyValueCharacteristic(
                    "test_read_only_value",
                    "00000002-0000-0000-0000-000000000001",
                    dbus_gatt::kCharacteristicFlagRead,
                    dbus_gatt::DBusGattVariantT(std::string("read only value"))
                    ),
                dbus_gatt::DBusGATT::Characteristic(
                    "test_char",
                    "00000003-0000-0000-0000-000000000001",
                    dbus_gatt::kCharacteristicFlagRead | dbus_gatt::kCharacteristicFlagWrite,
                    []() -> dbus_gatt::DBusGattVariantT {
                        return static_cast<int32_t>(some_value_);
                    },
                    [](const uint8_t* value, size_t size) {
                        // do something with data
                        return static_cast<int32_t>(0);
                    }
                    )
                )
            );
        }
    private:
        int32_t some_value_ = {0};
    };

    int main() {

        auto app = YourGATTAppName();

        app.start();

        // catch exit signall and call app.stop();

        return 0;
    }

    ```
2.  Create D-Bus permissions config file

    example /etc/dbus-1/system.d/`your-gatt-app-name`.conf
    ```ini
    <!DOCTYPE busconfig PUBLIC "-//freedesktop//DTD D-BUS Bus Configuration 1.0//EN" "http://www.freedesktop.org/standards/dbus/1.0/busconfig.dtd">
    <busconfig>
      <policy user="root">
        <allow own="your-gatt-app-bus-name"/>
        <allow send_destination="org.bluez"/>
      </policy>
      <policy at_console="true">
        <allow own="your-gatt-app-bus-name"/>
        <allow send_destination="org.bluez"/>
      </policy>
      <policy context="default">
      </policy>
    </busconfig>
    ```
3.  Create your app service configuration file

    example /etc/systemd/system/`your-gatt-app-name`.service:
    ```ini
    [Unit]
    Description=your app description
    After=bluetooth.service

    [Install]
    WantedBy=multi-user.target

    [Service]

    ExecStartPre=
    ExecStartPre=-btmgmt -i 0 power off
    ExecStartPre=-btmgmt -i 0 bredr off
    ExecStartPre=-btmgmt -i 0 le on
    ExecStartPre=-btmgmt -i 0 connectable on
    ExecStartPre=-btmgmt -i 0 advertising on
    ExecStartPre=-btmgmt -i 0 power on

    ExecStart=/path/to/your/app
    Type=simple
    WorkingDirectory=/
    Restart=always
    RestartSec=15
    KillSignal=SIGTERM
    StandardError=syslog
    ```
4.  Start your app service

    execute shell commands

    ```bash
    systemctl daemon-reload
    systemctl start your-gatt-app-name
    ```
