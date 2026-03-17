# Relays
We had a choice between electromechanical relays and solid-state relays.

| Electromechanical Relay                          | Solid-State Relay                |
| ------------------------------------------------ | -------------------------------- |
| Low on resistance $R_{on} < 500\mathrm{m\Omega}$ | Higher on-resistance $>2 \Omega$ |
| Can block high voltages                          | ~~Generally limited by voltage~~ |
| Large                                            | Roughly SOIC-Sized               |
| Switching requires special consideration         | Switchable with GPIO             |
|                                                  |                                  |
Due to the large resistances in the attenuator we deemed the 2R5 on resistance of the Panasonic AQW280SX to be negligible, however the ammeter uses a 100m shunt resistor, which would be greatly impacted by this. We chose to buffer the voltage over the shunt-resistor to eliminate this probem.