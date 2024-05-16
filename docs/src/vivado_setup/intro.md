## Which version of vivado?
vivado 2019.2 is the most recent version that is similar to the zynq book tutorials
- still xilinx vivado branded, with separate HLS, vivado & xilinx SDK.
- most popular version for the coursework, so less likely to have issues

Later verions are rebranded as vitis, and have some differences:
- xilinx SDK is gone
- for the vitis unified IDE, need to select the `xc7z020clg484-1` as the 
  `ZedBoard Zynq Evaluation and Development Kit` is gone

Tutorials are mostly for older versions of vivado, and the forums are sparse, 
if you do the project on the latest version, please contribute your learnings 
here.

## Vivado 2019 Date Bug
Xilinx doubted humanity would survive past 2022, and thus vivado 2019 will fail 
to launch properly in the current year of our lord.

In order to resolve this, either:
- Set your system time to 2021 (gives you a year to complete the coursework)
- Copy [`automg_patch_20220104.tcl`](https://github.com/OliverKillane/ADSD/tree/main/misc/automg_patch_20220104.tcl) into `C:\Programs\Vivado\2019.1\common\scripts` to solve the issue.
- Give up.
