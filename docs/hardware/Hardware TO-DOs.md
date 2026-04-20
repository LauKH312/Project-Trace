# TO-DO List - Hardware
## General
- [ ] Create and assign handsolder versions for all used applicable footprints
- [x] Assign Digikey Partnumbers to all parts ordered from Mouser
%% - [ ] Consider using Global Sourcing for Mouser Parts for PCBA %%
%% - [ ] Assign JLCPCB Partnumbers to all parts ordered for assembly %%
## Front-End
### Inputs & InAmp
- [-] Decide new gain-setting/settings
- [-] Compensate for on-resistance of switches?
- [x] Design AC-Coupling Circuit
- [x] Implement circuit using new 
## Digital
### ADC
- [x] Find voltage references

- Decided against creating a external clock source.
### User Interface
- [x] Decide upon interface
Use two rotary encoders,  two pushbuttons (off-(on)), power switch. Visual feedback with screen (128x64).
- [x] Integrate display
- [x] Connect buttons and encoders

### MCU
- [x] Decide which Nucleo board to use
- [x] Create Symbols
- [x] Create Footprint
- [x] Find SRAM IC
- [x] Create symbol and footprint for SRAM if necessary
## Power
_With BMS_
- [x] Decide on battery charging solution
- [x] Add screw-terminals

## Docs
- [ ] Update docs where applicable (rev2)