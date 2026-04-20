# Internal PCB Review 1
## Before Submitting Your Board for PCB Design Review
- [x] Check for unrouted nets
- [x] Repour all polygons
- [?] Ensure your board has a complete silkscreen, including:
	- [-] Company logo(s)
	- [?] Product logo(s)
	- [-] Copyright notice
	- [-] Warning/Hazard label(s) and Icons
	- [x] Connectors are labeled, and pinouts are called out where relevant
	- [-] QA/Test block with barcode, blank areas for a serial number, dates, and QA/Test checkmarks
	- [x] Board name, print date, and revision number
- [!] If there are designators on the silkscreen, then check:
	- [!] Each designator is close to and clearly identifies the associated component
		- C316-C321 messy
		- C336 intersects
		- JP303 intersects
		- TP201 (VCOM) silkscreen intersect
		- R311
	- [x] All designators are only in one or two orientations
	- [x] Text size and font will remain legible after the fabrications process
	- [!] ICs have pin one clearly marked, i.e., the pin one marker is not located under another component
		- U305 unclear
		- U306 might be unclear
		- U205 ambiguous
- [x] Update the PCB from its schematics to ensure the schematic and board are synchronized
- [x] Ensure the design rule report passes with no errors
- [?] Ensure there is a design rule to catch nets with only 1 pin
- [?] Ensure you have the board outline on a mechanical layer that will go to the board fabricator
- [-] Fiducials are present for the assembly and meet these criteria:
  - [-] Three board-level fiducials are included as a minimum
  - [-] Two fiducials are diagonally opposing each other across all extremely fine-pitch components
- [-] Mounting points have sufficient clearance for the chosen washer and screw head
- [-] If an enclosure model is available, it has been tested against the board to ensure there will be no interference between the enclosure and components
- [-] All components (including mechanical items) have accurate 3D models

## Layers
- [x] Check the layer stack and substrate heights meet the fabricator's specifications (or that the fabricator can meet your specifications)
- [x] Check the copper thickness on all layers matches the target fabricator's specifications (or that there is a callout on the documentation layer for the required copper thickness)
- [x] Ensure there is at least one continuous, unbroken ground plane
- [-] If your board has controlled impedance nets, they are correctly set up both on the layer stack and in the design rules
- [x] Check if any keep-out track matches the board shape
- [?] Check any board cutouts or slots used for a keep-out barrier to prevent nets from crossing milled areas
	- GND Plane cut under ADC?

// A PCB stackup table that was created as part of your front-end engineering specifications can be helpful here as it gives a visual comparison with the data in your PCB editor. A table provided by your fabrication house or manufacturer is the best resource, such as the example shown below.

## Signal Path
- [!] Ensure that the ground plane has sufficient current-carrying vias near connectors, and voltage/return sinks
	- Connector could use more
- [-] If required/relevant, ensure voltage planes and areas have sufficient connection vias for current requirements
- [x] Ensure tracks to reference planes are sufficiently wide for current requirements
- [x] Check there are sufficient quantities of vias for the current-carrying capacity of traces
- [x] Check the minimum track widths for all current-carrying nets are sufficient. Consider if a design rule is required
	- 3V3 track about the 'coupling' text can be made wider
	- 3V3 track above JP301 label has a sharp corner
- [x] Check all ground pins have a via to the ground plane
- [x] Check there is a continuous ground plane within one signal layer of any signal trace
- [-] Check any controlled impedance traces have the correct net rules and impedance profile
- [-] Check differential pair tracks are as close together as possible
- [-] Check differential pair tracks lengths are matched
- [-] Check all high-speed signals are length matched, specifically including:
  - [-] DDR
  - [-] PCIe
  - [-] Ethernet
  - [-] LVDS
  - [-] HDMI
  - [-] USB3+
  - [-] MIPI
- [-] Check every signal trace has a constant impedance along its length. If the trace changes layer, its impedance should stay the same
- [-] Check traces longer than 1/6th of the rise or fall time of the signal have been simulated:
- [!] Check termination resistors, or another termination is present to prevent ringing or overshoots
	- ADC SCLK, DOUT, FSO lacks termination
	- SPI3 SCLK, MOSI
- [ ] Check termination resistors are in the relevant locations
- [-] Check long traces that hug other traces that have been simulated for crosstalk
	- Display traces can be spaced more before conn
- [x] Check all high-speed traces run over a continuous ground pour
- [x] Ensure that no sensitive nets run under noisy components
- [x] Check that the vias for decoupling capacitors are not shared
- [x] Each decoupling capacitor requires its own via for VCC and for GND direct to reference planes

## Components
- [x] Check all through-hole pads are set to plated if they are going to be soldered
- [ ] Check there is sufficient clearance for the following:
  - [-] For pick and place heads in production
  - [ ] For hand assembly in prototypes
  - [ ] For soldering pen tip access if any rework is required
- [x] Check bypass capacitors are placed as close to IC power pins as possible (under 15mm)
- [!] Check crystal/oscillator clock sources are as close to the IC clock pins as possible
- [!] Check termination resistors are as close to the signal source as possible
- [-] Check EMI/RFI filtering is as close to the exit point (board edge, connector, shield) as possible
- [-] Check potentiometers increase signal/voltage when turned clockwise
- [ ] Check programmable devices have accessible programming header/pads
- [-] Check no high thermal mass components (e.g., large transformers/inductors) are located next to very small components
- [ ] Check component placement prioritizes short track lengths for high-speed signals
- [-] Check there are a sufficiently large copper area for heat-sinking high-dissipation devices, including:
  - [ ] Linear regulators
  - [-] Switched-mode power supplies (including LED drivers)
  - [-] High power LEDs
  - [-] High-frequency gate drivers
  - [-] MOSFETs
  - [-] Motor drivers
  - [-] Chargers
  - [-] High-speed microprocessors
  - [-] Power amplifiers

## Testing
- [x] Check that test pads are sufficiently far from the edge of the board to allow fixturing
- [-] Check test pads do not create stubs/impedance mismatch on high-speed nets
- [x] Check that component do not obstruct access to test pads for manual testing (e.g., using an oscilloscope probe) or automated testing (e.g., using a bed of nails)
- [x] Check test pads are clearly labeled for prototypes
- [!] Check any signal needed for testing or inspection has a test point
	- 3V3, 5V missing test points...
- [x] Check test points are all located on the same side of the board, ideally:
  - [-] The bottom of the board for access using a bed of nails fixtures
  - [x] The top of the board for manual access with test equipment (e.g., oscilloscope probes)

## Protection/EMI/EMC
- [-] Check appropriate creepage and clearance rules are set for all high-voltage nets
- [x] Check if separate earth tracks/paths are necessary for ESD
- [x] Check there are decoupling capacitors next to connectors and vias that may require them
- [x] Check TVS diodes or other ESD mitigation component pads are in series with the track to the component (i.e., any ESD event must pass through the component pad before getting to a sensitive device)
- [ ] Ensure there are no track stubs/net antennas going to test points or unused connector pins
- [ ] Ensure high-speed signals are routed as directly as possible—no scenic routes
- [x] Check any track carrying over 100mA has its width calculated to ensure it is of sufficient size for the current
- [ ] If the board will be fitted within an enclosure with little or no airflow, calculate or simulate the width for an internal layer instead of an external layer
- [ ] Check if an RF shield is required anywhere on the board and if it has a sufficient footprint
- [-] If you are using a two-layer board:
	- [-] Are there any ground loops?
	- [-] Is there a ground track of sufficient size for the return current of every device?
	- [-] Is there an unbroken ground pour under every high-speed trace?
	- [-] If you have multiple grounds, ensure that they are tied together at a single point only

## Panels
- [-] Ensure that there is sufficient frame area for conveyors and clamping
- [ ] Ensure that the silkscreen includes the following:
- [-] Blanks for QA/Testing marks
- [ ] Print date
- [-] Machine name
- [-] Company name
- [-] Panel barcode
- [x] Board part number and revision
- [-] Check panel fiducials are present
- [-] Check for an origin identification mark
- [-] Check impedance/layer/other test areas are present if required
- [-] Check V Score/Milling/tab layers are present and aligned with the board if they are not in the board file
- [?] Check the panel isn't too large for the board thickness and any milling (i.e., low flexibility/bounce)

## Comments
- Reassign designators?
- Maybe write pin-out to connector?
- Ensure Correct Orientation of Screw Terminal
- PCB legs
- Eliminate gnd fill antennas - swiss-cheese the thing
- 220R Resistors (for LED) are missing PN and Footprint - ignore PN if using resistors from lab.
- Extra GND via to U306

- AAF coeff