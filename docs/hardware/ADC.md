# ADC
The choice of ADC was a great challenge due to the limitations of microcontrollers. 
Available ADCs which sample in the tens of MS/s were to a high extent reliant on Serial LVDS, Parallel LVDS,  Serial CMOS interfaces, all of which are not available to us due to the limitations of microcontrollers.
With a greater amount of development time, the problem can be solved using an FPGA. Due to our serious time constraints this is out of the question.

Initially we had wanted to use only one ADC (the front-runner was the ADS1605). 
Later we chose to solve the problem by compromising on bit width, keeping a second ADC with high precision and no strict speed requirements for DC and slow AC measurements.%% The faster-sampling less precise ADC is named **AC-ADC** and the slower highly precise ADC is named the **DC-ADC**. %%
Due to time and cost constraints we had to revert to using one  ADC, compromising instead on speed.

%% The loss of precision for the AC-ADC is remedied by adding new elements to the signal conditioning, allowing greater voltage resolution. This includes variable offset (requiring a precise DAC),  AC coupling and more gain settings for the InAmp. 
 %%

## ADC
We identified the following ADCs to be the most suitable candidates for the AC-ADC.

| **PARAMETER**         | **ADS1605** | ADS1602  | AD4001BRMZ | **ADC3542(ADC3543)** | **ADS850** |
| --------------------- | ----------- | -------- | ---------- | -------------------- | ---------- |
| **Max Sampling Rate** | 5 MS/s      | 2.5 MS/s | 2 MS/s     | 25(65) MS/s          | 10 MS/s    |
| **Bit Width**         | 16b         | 16b      | 16b        | 14b                  | 14b        |
| **VDD**               | 5V          | 5.0V     | 1.8V       | 1.8V                 | 5.0V       |
| **VDDIO**             | 3.3V        | 3.3V     | 3.3V       | 1.8V                 | 3.3V       |
| **Interfaces**        | PCMOS       | SPI      | SPI        | SDR,DDR,PCMOS        | PCMOS      |
| Cost (no VAT)         | 387DKK      | 151DKK   | 145DKK     |                      |            |

%% In terms of bandwidth the ADC3542/ADC3543 is the superior option, providing the possibility of sampling the fastest. It is due to the need for special signal conditioning and logic level shifting needed to accommodate its 1.8V supply that it was rejected. 
Second best is the ADS850, which we initially chose, due to its okay-ish sample-rate and its easy integration, which made it suitable for this project.
The AD850 is beat by the ADS1605 on bit-depth and not surprisingly on SNR as a result. Our hope is that the extra signal conditioning will make this difference insignificant in most applications.
 %%
The 14b options were only seriously considered when designing the dual-ADC version, however using a single ADC, the low bit-depth is considered too limiting for the precision of DC measurements.
d