# Setup Correct Keycode in Linux 

## create x220-x230kb.service in /etc/systemd/system

```
[Unit]
Description=Change keycodes for X220 using X230 keyboard

[Service]
Type=oneshot
# DELETE
ExecStart=/usr/bin/setkeycodes e049 111
# INSERT
ExecStart=/usr/bin/setkeycodes e051 110
# END
ExecStart=/usr/bin/setkeycodes e04f 107
# HOME
ExecStart=/usr/bin/setkeycodes e053 102
# PGUP
ExecStart=/usr/bin/setkeycodes e06a 104
# PGDOWN
ExecStart=/usr/bin/setkeycodes e069 109
# FN+HOME, BRIGHTNESS UP
ExecStart=/usr/bin/setkeycodes e063 225

# FN+F9
#ExecStart=/usr/bin/setkeycodes 6f 225

[Install]
WantedBy=multi-user.target

```

## enable x220-x230kb service

```
sudo chmod +x /etc/systemd/system/x220-x230kb.service
sudo systemctl enable x220-x230kb
```

## IT WORKS ON BOTH CONSOLE & X WINDOWS
