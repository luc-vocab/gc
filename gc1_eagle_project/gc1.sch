<?xml version="1.0" encoding="utf-8"?>
<!DOCTYPE eagle SYSTEM "eagle.dtd">
<eagle version="6.4">
<drawing>
<settings>
<setting alwaysvectorfont="no"/>
<setting verticaltext="up"/>
</settings>
<grid distance="0.1" unitdist="inch" unit="inch" style="lines" multiple="1" display="no" altdistance="0.01" altunitdist="inch" altunit="inch"/>
<layers>
<layer number="1" name="Top" color="4" fill="1" visible="no" active="no"/>
<layer number="16" name="Bottom" color="1" fill="1" visible="no" active="no"/>
<layer number="17" name="Pads" color="2" fill="1" visible="no" active="no"/>
<layer number="18" name="Vias" color="2" fill="1" visible="no" active="no"/>
<layer number="19" name="Unrouted" color="6" fill="1" visible="no" active="no"/>
<layer number="20" name="Dimension" color="15" fill="1" visible="no" active="no"/>
<layer number="21" name="tPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="22" name="bPlace" color="7" fill="1" visible="no" active="no"/>
<layer number="23" name="tOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="24" name="bOrigins" color="15" fill="1" visible="no" active="no"/>
<layer number="25" name="tNames" color="7" fill="1" visible="no" active="no"/>
<layer number="26" name="bNames" color="7" fill="1" visible="no" active="no"/>
<layer number="27" name="tValues" color="7" fill="1" visible="no" active="no"/>
<layer number="28" name="bValues" color="7" fill="1" visible="no" active="no"/>
<layer number="29" name="tStop" color="7" fill="3" visible="no" active="no"/>
<layer number="30" name="bStop" color="7" fill="6" visible="no" active="no"/>
<layer number="31" name="tCream" color="7" fill="4" visible="no" active="no"/>
<layer number="32" name="bCream" color="7" fill="5" visible="no" active="no"/>
<layer number="33" name="tFinish" color="6" fill="3" visible="no" active="no"/>
<layer number="34" name="bFinish" color="6" fill="6" visible="no" active="no"/>
<layer number="35" name="tGlue" color="7" fill="4" visible="no" active="no"/>
<layer number="36" name="bGlue" color="7" fill="5" visible="no" active="no"/>
<layer number="37" name="tTest" color="7" fill="1" visible="no" active="no"/>
<layer number="38" name="bTest" color="7" fill="1" visible="no" active="no"/>
<layer number="39" name="tKeepout" color="4" fill="11" visible="no" active="no"/>
<layer number="40" name="bKeepout" color="1" fill="11" visible="no" active="no"/>
<layer number="41" name="tRestrict" color="4" fill="10" visible="no" active="no"/>
<layer number="42" name="bRestrict" color="1" fill="10" visible="no" active="no"/>
<layer number="43" name="vRestrict" color="2" fill="10" visible="no" active="no"/>
<layer number="44" name="Drills" color="7" fill="1" visible="no" active="no"/>
<layer number="45" name="Holes" color="7" fill="1" visible="no" active="no"/>
<layer number="46" name="Milling" color="3" fill="1" visible="no" active="no"/>
<layer number="47" name="Measures" color="7" fill="1" visible="no" active="no"/>
<layer number="48" name="Document" color="7" fill="1" visible="no" active="no"/>
<layer number="49" name="Reference" color="7" fill="1" visible="no" active="no"/>
<layer number="51" name="tDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="52" name="bDocu" color="7" fill="1" visible="no" active="no"/>
<layer number="91" name="Nets" color="2" fill="1" visible="yes" active="yes"/>
<layer number="92" name="Busses" color="1" fill="1" visible="yes" active="yes"/>
<layer number="93" name="Pins" color="2" fill="1" visible="no" active="yes"/>
<layer number="94" name="Symbols" color="4" fill="1" visible="yes" active="yes"/>
<layer number="95" name="Names" color="7" fill="1" visible="yes" active="yes"/>
<layer number="96" name="Values" color="7" fill="1" visible="yes" active="yes"/>
<layer number="97" name="Info" color="7" fill="1" visible="yes" active="yes"/>
<layer number="98" name="Guide" color="6" fill="1" visible="yes" active="yes"/>
</layers>
<schematic xreflabel="%F%N/%S.%C%R" xrefpart="/%S.%C%R">
<libraries>
<library name="gc1_parts">
<packages>
<package name="ARDUINO_MICRO">
<pad name="12/A12" x="0" y="0" drill="0.8"/>
<pad name="MOSI" x="40.64" y="0" drill="0.8"/>
<pad name="SCK" x="40.64" y="-15.24" drill="0.8" rot="R90"/>
<pad name="11" x="2.54" y="0" drill="0.8"/>
<pad name="10/A11" x="5.08" y="0" drill="0.8"/>
<pad name="9/A10" x="7.62" y="0" drill="0.8"/>
<pad name="8/A9" x="10.16" y="0" drill="0.8"/>
<pad name="7" x="12.7" y="0" drill="0.8"/>
<pad name="6/A7" x="15.24" y="0" drill="0.8"/>
<pad name="5" x="17.78" y="0" drill="0.8"/>
<pad name="4/A6" x="20.32" y="0" drill="0.8"/>
<pad name="3/SCL" x="22.86" y="0" drill="0.8"/>
<pad name="2/SDA" x="25.4" y="0" drill="0.8"/>
<pad name="GND" x="27.94" y="0" drill="0.8"/>
<pad name="RESET" x="30.48" y="0" drill="0.8"/>
<pad name="0/RX" x="33.02" y="0" drill="0.8"/>
<pad name="1/TX" x="35.56" y="0" drill="0.8"/>
<pad name="RX_LED/SS" x="38.1" y="0" drill="0.8"/>
<pad name="13" x="0" y="-15.24" drill="0.8"/>
<pad name="3.3V" x="2.54" y="-15.24" drill="0.8"/>
<pad name="AREF" x="5.08" y="-15.24" drill="0.8"/>
<pad name="A0" x="7.62" y="-15.24" drill="0.8"/>
<pad name="A1" x="10.16" y="-15.24" drill="0.8"/>
<pad name="A2" x="12.7" y="-15.24" drill="0.8"/>
<pad name="A3" x="15.24" y="-15.24" drill="0.8"/>
<pad name="A4" x="17.78" y="-15.24" drill="0.8"/>
<pad name="A5" x="20.32" y="-15.24" drill="0.8"/>
<pad name="+5V" x="27.94" y="-15.24" drill="0.8"/>
<pad name="RESET2" x="30.48" y="-15.24" drill="0.8"/>
<pad name="GND2" x="33.02" y="-15.24" drill="0.8"/>
<pad name="VIN" x="35.56" y="-15.24" drill="0.8"/>
<pad name="MISO" x="38.1" y="-15.24" drill="0.8"/>
<wire x1="-3.81" y1="1.27" x2="44.3865" y2="1.27" width="0.127" layer="21"/>
<wire x1="-3.81" y1="1.27" x2="-3.81" y2="-3.81" width="0.127" layer="21"/>
<wire x1="-3.81" y1="-3.81" x2="-3.81" y2="-11.43" width="0.127" layer="21"/>
<wire x1="-3.81" y1="-11.43" x2="-3.81" y2="-16.5735" width="0.127" layer="21"/>
<wire x1="-3.81" y1="-16.5735" x2="43.2435" y2="-16.5735" width="0.127" layer="21"/>
<wire x1="43.307" y1="-16.5735" x2="44.3865" y2="-16.5735" width="0.127" layer="21"/>
<wire x1="44.3865" y1="-16.5735" x2="44.3865" y2="1.27" width="0.127" layer="21"/>
<pad name="UNUSED1" x="22.86" y="-15.24" drill="0.8"/>
<pad name="UNUSED2" x="25.4" y="-15.24" drill="0.8"/>
<text x="30.226" y="-7.366" size="1.27" layer="21">&gt;NAME</text>
<text x="33.274" y="-13.462" size="0.6096" layer="21" rot="R90">GND</text>
<text x="36.068" y="-13.716" size="0.6096" layer="21" rot="R90">VIN</text>
<text x="23.241" y="-7.747" size="0.6096" layer="21" rot="R90">MICROSD_CS</text>
<text x="18.288" y="-7.366" size="0.6096" layer="21" rot="R90">SOUND_PIN</text>
<text x="13.208" y="-7.874" size="0.6096" layer="21" rot="R90">CONTROL_PIN</text>
<text x="7.874" y="-13.335" size="0.6096" layer="21" rot="R90">SENSOR_SIG</text>
<text x="40.894" y="-13.462" size="0.6096" layer="21" rot="R90">MICROSD_CLK</text>
<text x="38.608" y="-13.716" size="0.6096" layer="21" rot="R90">MICROSD_DO</text>
<text x="40.894" y="-6.604" size="0.6096" layer="21" rot="R90">MICROSD_DI</text>
<text x="28.194" y="-13.462" size="0.6096" layer="21" rot="R90">ARDUINO_+5V</text>
<wire x1="-3.81" y1="-3.81" x2="-6.35" y2="-3.81" width="0.127" layer="21"/>
<wire x1="-6.35" y1="-3.81" x2="-6.35" y2="-11.43" width="0.127" layer="21"/>
<wire x1="-6.35" y1="-11.43" x2="-3.81" y2="-11.43" width="0.127" layer="21"/>
</package>
<package name="MICROSD">
<wire x1="0" y1="0" x2="6.35" y2="0" width="0.127" layer="21"/>
<wire x1="6.35" y1="0" x2="20.32" y2="0" width="0.127" layer="21"/>
<wire x1="20.32" y1="0" x2="25.4" y2="0" width="0.127" layer="21"/>
<wire x1="25.4" y1="0" x2="25.4" y2="-31.75" width="0.127" layer="21"/>
<wire x1="25.4" y1="-31.75" x2="0" y2="-31.75" width="0.127" layer="21"/>
<wire x1="0" y1="-31.75" x2="0" y2="0" width="0.127" layer="21"/>
<pad name="CD" x="21.59" y="-30.48" drill="0.8"/>
<pad name="CS" x="19.05" y="-30.48" drill="0.8"/>
<pad name="DI" x="16.51" y="-30.48" drill="0.8"/>
<pad name="DO" x="13.97" y="-30.48" drill="0.8"/>
<pad name="CLK" x="11.43" y="-30.48" drill="0.8"/>
<pad name="GND" x="8.89" y="-30.48" drill="0.8"/>
<pad name="3V" x="6.35" y="-30.48" drill="0.8"/>
<pad name="5V" x="3.81" y="-30.48" drill="0.8"/>
<text x="7.62" y="-12.7" size="1.27" layer="21">MICROSD</text>
<text x="4.064" y="-29.21" size="0.6096" layer="21" rot="R90">ARDUINO_+5V</text>
<text x="9.144" y="-29.21" size="0.6096" layer="21" rot="R90">GND</text>
<text x="11.684" y="-29.21" size="0.6096" layer="21" rot="R90">CLK</text>
<text x="14.224" y="-29.21" size="0.6096" layer="21" rot="R90">DO</text>
<text x="16.764" y="-29.21" size="0.6096" layer="21" rot="R90">DI</text>
<text x="19.304" y="-29.21" size="0.6096" layer="21" rot="R90">CS</text>
<wire x1="6.35" y1="0" x2="6.35" y2="2.54" width="0.127" layer="21"/>
<wire x1="6.35" y1="2.54" x2="20.32" y2="2.54" width="0.127" layer="21"/>
<wire x1="20.32" y1="2.54" x2="20.32" y2="0" width="0.127" layer="21"/>
<text x="10.414" y="-1.524" size="0.6096" layer="21">microsd card</text>
</package>
<package name="MUSCLE_SENSOR">
<wire x1="0" y1="0" x2="1.27" y2="0" width="0.127" layer="21"/>
<wire x1="1.27" y1="0" x2="3.81" y2="0" width="0.127" layer="21"/>
<wire x1="3.81" y1="0" x2="25.4" y2="0" width="0.127" layer="21"/>
<wire x1="25.4" y1="0" x2="25.4" y2="-25.4" width="0.127" layer="21"/>
<wire x1="25.4" y1="-25.4" x2="0" y2="-25.4" width="0.127" layer="21"/>
<wire x1="0" y1="-25.4" x2="0" y2="0" width="0.127" layer="21"/>
<pad name="+VS" x="1.27" y="-24.13" drill="0.8"/>
<pad name="GND" x="3.81" y="-24.13" drill="0.8"/>
<pad name="-VS" x="6.35" y="-24.13" drill="0.8"/>
<pad name="SIG" x="13.97" y="-24.13" drill="0.8"/>
<pad name="SIG_GND" x="16.51" y="-24.13" drill="0.8"/>
<text x="5.08" y="-11.43" size="1.27" layer="21">MUSCLE_SENSOR</text>
<text x="4.064" y="-22.86" size="0.6096" layer="21" rot="R90">GND</text>
<text x="1.524" y="-22.86" size="0.6096" layer="21" rot="R90">+9V_BATT_OUT</text>
<text x="6.604" y="-22.86" size="0.6096" layer="21" rot="R90">-9V_BATT_OUT</text>
<text x="14.224" y="-22.86" size="0.6096" layer="21" rot="R90">SENSOR_SIG</text>
<text x="16.764" y="-23.114" size="0.6096" layer="21" rot="R90">GND</text>
<wire x1="1.27" y1="0" x2="1.27" y2="3.81" width="0.127" layer="21"/>
<wire x1="1.27" y1="3.81" x2="3.81" y2="3.81" width="0.127" layer="21"/>
<wire x1="3.81" y1="3.81" x2="3.81" y2="0" width="0.127" layer="21"/>
<text x="1.016" y="-1.524" size="0.6096" layer="21">3.5mm jack</text>
</package>
<package name="SWITCH_SS22SDH2">
<wire x1="0" y1="0" x2="7.62" y2="0" width="0.127" layer="21"/>
<wire x1="7.62" y1="0" x2="7.62" y2="-3.81" width="0.127" layer="21"/>
<wire x1="7.62" y1="-3.81" x2="7.62" y2="-5.08" width="0.127" layer="21"/>
<wire x1="7.62" y1="-6.35" x2="7.62" y2="-10.16" width="0.127" layer="21"/>
<wire x1="7.62" y1="-10.16" x2="0" y2="-10.16" width="0.127" layer="21"/>
<wire x1="0" y1="-10.16" x2="0" y2="0" width="0.127" layer="21"/>
<pad name="4" x="2.54" y="-2.54" drill="0.8"/>
<pad name="1" x="5.08" y="-2.54" drill="0.8"/>
<pad name="5" x="2.54" y="-5.08" drill="0.8"/>
<pad name="2" x="5.08" y="-5.08" drill="0.8"/>
<pad name="6" x="2.54" y="-7.62" drill="0.8"/>
<pad name="3" x="5.08" y="-7.62" drill="0.8"/>
<wire x1="7.62" y1="-3.81" x2="10.16" y2="-3.81" width="0.127" layer="21"/>
<wire x1="10.16" y1="-3.81" x2="10.16" y2="-6.35" width="0.127" layer="21"/>
<wire x1="10.16" y1="-6.35" x2="7.62" y2="-6.35" width="0.127" layer="21"/>
<wire x1="7.62" y1="-6.35" x2="7.62" y2="-5.08" width="0.127" layer="21"/>
</package>
<package name="9V_CONNECTOR">
<pad name="+9V" x="0" y="0" drill="0.8"/>
<pad name="-" x="2.54" y="0" drill="0.8"/>
<text x="-2.54" y="-2.54" size="1.27" layer="21">+9v</text>
<text x="2.54" y="-2.54" size="1.27" layer="21">-</text>
</package>
<package name="SWITCH_SS12SDH2">
<pad name="1" x="2.54" y="-2.54" drill="0.8"/>
<pad name="2" x="2.54" y="-5.08" drill="0.8"/>
<pad name="3" x="2.54" y="-7.62" drill="0.8"/>
<wire x1="0" y1="0" x2="0" y2="-10.16" width="0.127" layer="21"/>
<wire x1="0" y1="0" x2="5.08" y2="0" width="0.127" layer="21"/>
<wire x1="5.08" y1="0" x2="5.08" y2="-3.81" width="0.127" layer="21"/>
<wire x1="5.08" y1="-3.81" x2="5.08" y2="-6.35" width="0.127" layer="21"/>
<wire x1="5.08" y1="-6.35" x2="5.08" y2="-10.16" width="0.127" layer="21"/>
<wire x1="5.08" y1="-10.16" x2="0" y2="-10.16" width="0.127" layer="21"/>
<text x="1.016" y="-6.096" size="0.6096" layer="21" rot="R90">switch</text>
<wire x1="5.08" y1="-3.81" x2="7.62" y2="-3.81" width="0.127" layer="21"/>
<wire x1="7.62" y1="-3.81" x2="7.62" y2="-6.35" width="0.127" layer="21"/>
<wire x1="7.62" y1="-6.35" x2="5.08" y2="-6.35" width="0.127" layer="21"/>
</package>
<package name="35RAPC4BVN4">
<wire x1="-1.27" y1="-2.54" x2="-1.27" y2="-5.08" width="0.127" layer="21"/>
<wire x1="-1.27" y1="-5.08" x2="12.7" y2="-5.08" width="0.127" layer="21"/>
<wire x1="12.7" y1="-5.08" x2="12.7" y2="3.81" width="0.127" layer="21"/>
<wire x1="12.7" y1="3.81" x2="-1.27" y2="3.81" width="0.127" layer="21"/>
<wire x1="-1.27" y1="3.81" x2="-5.08" y2="3.81" width="0.127" layer="21"/>
<wire x1="-5.08" y1="3.81" x2="-5.08" y2="-2.54" width="0.127" layer="21"/>
<wire x1="-5.08" y1="-2.54" x2="-1.27" y2="-2.54" width="0.127" layer="21"/>
<pad name="1" x="0" y="0" drill="2"/>
<pad name="2" x="2.4638" y="1.9812" drill="2"/>
<pad name="4" x="8.0518" y="1.9812" drill="2"/>
<text x="1.27" y="-3.81" size="1.27" layer="21">3.5MM JACK</text>
</package>
<package name="66WR1KLF">
<pad name="2" x="0" y="0" drill="0.8"/>
<pad name="1" x="2.54" y="0" drill="0.8"/>
<pad name="3" x="-2.54" y="0" drill="0.8"/>
<wire x1="-5.08" y1="2.54" x2="-5.08" y2="-2.54" width="0.127" layer="21"/>
<wire x1="-5.08" y1="-2.54" x2="5.08" y2="-2.54" width="0.127" layer="21"/>
<wire x1="5.08" y1="-2.54" x2="5.08" y2="2.54" width="0.127" layer="21"/>
<wire x1="5.08" y1="2.54" x2="-5.08" y2="2.54" width="0.127" layer="21"/>
<text x="-2.54" y="-1.905" size="1.27" layer="21">&gt;name</text>
</package>
<package name="10K_RESISTOR">
<pad name="1" x="0" y="0" drill="0.8"/>
<pad name="2" x="12.7" y="0" drill="0.8"/>
<text x="3.175" y="-0.635" size="1.27" layer="21">&gt;NAME</text>
</package>
</packages>
<symbols>
<symbol name="ARDUINO_MICRO">
<wire x1="0" y1="0" x2="0" y2="-17.78" width="0.254" layer="94"/>
<pin name="12/A12" x="2.54" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="MOSI" x="43.18" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="SCK" x="43.18" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="11" x="5.08" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="10/A11" x="7.62" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="9/A10" x="10.16" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="8/A9" x="12.7" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="7" x="15.24" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="6/A7" x="17.78" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="5" x="20.32" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="4/A6" x="22.86" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="3/SCL" x="25.4" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="2/SDA" x="27.94" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="GND" x="30.48" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="RESET" x="33.02" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="0/RX" x="35.56" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="1/TX" x="38.1" y="5.08" visible="pin" length="middle" rot="R270"/>
<pin name="RX_LED/SS" x="40.64" y="5.08" visible="pin" length="middle" rot="R270"/>
<wire x1="0" y1="0" x2="45.72" y2="0" width="0.254" layer="94"/>
<wire x1="45.72" y1="0" x2="45.72" y2="-17.78" width="0.254" layer="94"/>
<wire x1="45.72" y1="-17.78" x2="0" y2="-17.78" width="0.254" layer="94"/>
<pin name="13" x="2.54" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="3.3V" x="5.08" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="AREF" x="7.62" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="A0" x="10.16" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="A1" x="12.7" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="A2" x="15.24" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="A3" x="17.78" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="A4" x="20.32" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="A5" x="22.86" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="+5V" x="30.48" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="RESET2" x="33.02" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="GND2" x="35.56" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="VIN" x="38.1" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="MISO" x="40.64" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="UNUSED1" x="25.4" y="-22.86" visible="pin" length="middle" rot="R90"/>
<pin name="UNUSED2" x="27.94" y="-22.86" visible="pin" length="middle" rot="R90"/>
</symbol>
<symbol name="MICROSD">
<wire x1="0" y1="0" x2="25.4" y2="0" width="0.254" layer="94"/>
<wire x1="25.4" y1="0" x2="25.4" y2="-31.75" width="0.254" layer="94"/>
<wire x1="25.4" y1="-31.75" x2="0" y2="-31.75" width="0.254" layer="94"/>
<wire x1="0" y1="-31.75" x2="0" y2="0" width="0.254" layer="94"/>
<pin name="CD" x="21.59" y="-36.83" visible="pin" length="middle" rot="R90"/>
<pin name="CS" x="19.05" y="-36.83" visible="pin" length="middle" rot="R90"/>
<pin name="DI" x="16.51" y="-36.83" visible="pin" length="middle" rot="R90"/>
<pin name="DO" x="13.97" y="-36.83" visible="pin" length="middle" rot="R90"/>
<pin name="CLK" x="11.43" y="-36.83" visible="pin" length="middle" rot="R90"/>
<pin name="GND" x="8.89" y="-36.83" visible="pin" length="middle" rot="R90"/>
<pin name="3V" x="6.35" y="-36.83" visible="pin" length="middle" rot="R90"/>
<pin name="5V" x="3.81" y="-36.83" visible="pin" length="middle" rot="R90"/>
</symbol>
<symbol name="MUSCLE_SENSOR">
<wire x1="0" y1="0" x2="25.4" y2="0" width="0.254" layer="94"/>
<wire x1="25.4" y1="0" x2="25.4" y2="-25.4" width="0.254" layer="94"/>
<wire x1="25.4" y1="-25.4" x2="0" y2="-25.4" width="0.254" layer="94"/>
<wire x1="0" y1="-25.4" x2="0" y2="0" width="0.254" layer="94"/>
<pin name="+VS" x="2.54" y="-30.48" visible="pin" length="middle" rot="R90"/>
<pin name="GND" x="5.08" y="-30.48" visible="pin" length="middle" rot="R90"/>
<pin name="-VS" x="7.62" y="-30.48" visible="pin" length="middle" rot="R90"/>
<pin name="SIG" x="15.24" y="-30.48" visible="pin" length="middle" rot="R90"/>
<pin name="SIG_GND" x="17.78" y="-30.48" visible="pin" length="middle" rot="R90"/>
</symbol>
<symbol name="SWITCH_SS22SDH2">
<wire x1="0" y1="0" x2="7.62" y2="0" width="0.254" layer="94"/>
<wire x1="7.62" y1="0" x2="7.62" y2="-10.16" width="0.254" layer="94"/>
<wire x1="7.62" y1="-10.16" x2="0" y2="-10.16" width="0.254" layer="94"/>
<wire x1="0" y1="-10.16" x2="0" y2="0" width="0.254" layer="94"/>
<pin name="4" x="-5.08" y="-2.54" visible="pin" length="middle"/>
<pin name="5" x="-5.08" y="-5.08" visible="pin" length="middle"/>
<pin name="6" x="-5.08" y="-7.62" visible="pin" length="middle"/>
<pin name="1" x="12.7" y="-2.54" visible="pin" length="middle" rot="R180"/>
<pin name="2" x="12.7" y="-5.08" visible="pin" length="middle" rot="R180"/>
<pin name="3" x="12.7" y="-7.62" visible="pin" length="middle" rot="R180"/>
<text x="-4.064" y="-12.7" size="1.27" layer="94">SWITCH_SS22SDH2</text>
</symbol>
<symbol name="9V_CONNECTOR">
<pin name="+9V" x="2.54" y="5.08" visible="pin" length="middle"/>
<pin name="-" x="2.54" y="0" visible="pin" length="middle"/>
</symbol>
<symbol name="SWITCH_SS12SDH2">
<wire x1="0" y1="0" x2="5.08" y2="0" width="0.254" layer="94"/>
<wire x1="5.08" y1="0" x2="5.08" y2="-10.16" width="0.254" layer="94"/>
<wire x1="5.08" y1="-10.16" x2="0" y2="-10.16" width="0.254" layer="94"/>
<wire x1="0" y1="-10.16" x2="0" y2="0" width="0.254" layer="94"/>
<pin name="1" x="-5.08" y="-2.54" visible="pin" length="middle"/>
<pin name="2" x="-5.08" y="-5.08" visible="pin" length="middle"/>
<pin name="3" x="-5.08" y="-7.62" visible="pin" length="middle"/>
</symbol>
<symbol name="35RAPC2BVN4">
<wire x1="-5.08" y1="7.62" x2="7.62" y2="7.62" width="0.254" layer="94"/>
<wire x1="7.62" y1="7.62" x2="7.62" y2="-7.62" width="0.254" layer="94"/>
<wire x1="7.62" y1="-7.62" x2="-5.08" y2="-7.62" width="0.254" layer="94"/>
<wire x1="-5.08" y1="-7.62" x2="-5.08" y2="7.62" width="0.254" layer="94"/>
<text x="0.254" y="-3.302" size="1.27" layer="94">&gt;name</text>
<pin name="1" x="-10.16" y="5.08" visible="pin" length="middle"/>
<pin name="2" x="-10.16" y="0" visible="pin" length="middle"/>
<pin name="4" x="-10.16" y="-5.08" visible="pin" length="middle"/>
</symbol>
<symbol name="66WR1KLF">
<wire x1="0" y1="5.08" x2="10.16" y2="5.08" width="0.254" layer="94"/>
<wire x1="10.16" y1="5.08" x2="10.16" y2="-5.08" width="0.254" layer="94"/>
<wire x1="10.16" y1="-5.08" x2="0" y2="-5.08" width="0.254" layer="94"/>
<wire x1="0" y1="-5.08" x2="0" y2="5.08" width="0.254" layer="94"/>
<pin name="1" x="-5.08" y="2.54" visible="pin" length="middle"/>
<pin name="2" x="-5.08" y="0" visible="pin" length="middle"/>
<pin name="3" x="-5.08" y="-2.54" visible="pin" length="middle"/>
</symbol>
<symbol name="10K_RESISTOR">
<wire x1="0" y1="2.54" x2="7.62" y2="2.54" width="0.254" layer="94"/>
<wire x1="7.62" y1="2.54" x2="7.62" y2="-5.08" width="0.254" layer="94"/>
<wire x1="7.62" y1="-5.08" x2="0" y2="-5.08" width="0.254" layer="94"/>
<wire x1="0" y1="-5.08" x2="0" y2="2.54" width="0.254" layer="94"/>
<pin name="1" x="-5.08" y="0" visible="pin" length="middle"/>
<pin name="2" x="-5.08" y="-2.54" visible="pin" length="middle"/>
<text x="8.382" y="-2.286" size="1.27" layer="94">&gt;name</text>
</symbol>
</symbols>
<devicesets>
<deviceset name="ARDUINO_MICRO">
<gates>
<gate name="G$1" symbol="ARDUINO_MICRO" x="-10.16" y="-2.54"/>
</gates>
<devices>
<device name="" package="ARDUINO_MICRO">
<connects>
<connect gate="G$1" pin="+5V" pad="+5V"/>
<connect gate="G$1" pin="0/RX" pad="0/RX"/>
<connect gate="G$1" pin="1/TX" pad="1/TX"/>
<connect gate="G$1" pin="10/A11" pad="10/A11"/>
<connect gate="G$1" pin="11" pad="11"/>
<connect gate="G$1" pin="12/A12" pad="12/A12"/>
<connect gate="G$1" pin="13" pad="13"/>
<connect gate="G$1" pin="2/SDA" pad="2/SDA"/>
<connect gate="G$1" pin="3.3V" pad="3.3V"/>
<connect gate="G$1" pin="3/SCL" pad="3/SCL"/>
<connect gate="G$1" pin="4/A6" pad="4/A6"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6/A7" pad="6/A7"/>
<connect gate="G$1" pin="7" pad="7"/>
<connect gate="G$1" pin="8/A9" pad="8/A9"/>
<connect gate="G$1" pin="9/A10" pad="9/A10"/>
<connect gate="G$1" pin="A0" pad="A0"/>
<connect gate="G$1" pin="A1" pad="A1"/>
<connect gate="G$1" pin="A2" pad="A2"/>
<connect gate="G$1" pin="A3" pad="A3"/>
<connect gate="G$1" pin="A4" pad="A4"/>
<connect gate="G$1" pin="A5" pad="A5"/>
<connect gate="G$1" pin="AREF" pad="AREF"/>
<connect gate="G$1" pin="GND" pad="GND"/>
<connect gate="G$1" pin="GND2" pad="GND2"/>
<connect gate="G$1" pin="MISO" pad="MISO"/>
<connect gate="G$1" pin="MOSI" pad="MOSI"/>
<connect gate="G$1" pin="RESET" pad="RESET"/>
<connect gate="G$1" pin="RESET2" pad="RESET2"/>
<connect gate="G$1" pin="RX_LED/SS" pad="RX_LED/SS"/>
<connect gate="G$1" pin="SCK" pad="SCK"/>
<connect gate="G$1" pin="UNUSED1" pad="UNUSED1"/>
<connect gate="G$1" pin="UNUSED2" pad="UNUSED2"/>
<connect gate="G$1" pin="VIN" pad="VIN"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="MICROSD">
<gates>
<gate name="G$1" symbol="MICROSD" x="0" y="0"/>
</gates>
<devices>
<device name="" package="MICROSD">
<connects>
<connect gate="G$1" pin="3V" pad="3V"/>
<connect gate="G$1" pin="5V" pad="5V"/>
<connect gate="G$1" pin="CD" pad="CD"/>
<connect gate="G$1" pin="CLK" pad="CLK"/>
<connect gate="G$1" pin="CS" pad="CS"/>
<connect gate="G$1" pin="DI" pad="DI"/>
<connect gate="G$1" pin="DO" pad="DO"/>
<connect gate="G$1" pin="GND" pad="GND"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="MUSCLE_SENSOR">
<gates>
<gate name="G$1" symbol="MUSCLE_SENSOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="MUSCLE_SENSOR">
<connects>
<connect gate="G$1" pin="+VS" pad="+VS"/>
<connect gate="G$1" pin="-VS" pad="-VS"/>
<connect gate="G$1" pin="GND" pad="GND"/>
<connect gate="G$1" pin="SIG" pad="SIG"/>
<connect gate="G$1" pin="SIG_GND" pad="SIG_GND"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="SWITCH_SS22SDH2">
<gates>
<gate name="G$1" symbol="SWITCH_SS22SDH2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SWITCH_SS22SDH2">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
<connect gate="G$1" pin="4" pad="4"/>
<connect gate="G$1" pin="5" pad="5"/>
<connect gate="G$1" pin="6" pad="6"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="9V_CONNECTOR">
<gates>
<gate name="G$1" symbol="9V_CONNECTOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="9V_CONNECTOR">
<connects>
<connect gate="G$1" pin="+9V" pad="+9V"/>
<connect gate="G$1" pin="-" pad="-"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="SWITCH_SS12SDH2">
<gates>
<gate name="G$1" symbol="SWITCH_SS12SDH2" x="0" y="0"/>
</gates>
<devices>
<device name="" package="SWITCH_SS12SDH2">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="35RAPC2BVN4">
<gates>
<gate name="G$1" symbol="35RAPC2BVN4" x="0" y="0"/>
</gates>
<devices>
<device name="" package="35RAPC4BVN4">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="4" pad="4"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="66WR1KLF">
<gates>
<gate name="G$1" symbol="66WR1KLF" x="-5.08" y="0"/>
</gates>
<devices>
<device name="" package="66WR1KLF">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
<connect gate="G$1" pin="3" pad="3"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
<deviceset name="10K_RESISTOR">
<gates>
<gate name="G$1" symbol="10K_RESISTOR" x="0" y="0"/>
</gates>
<devices>
<device name="" package="10K_RESISTOR">
<connects>
<connect gate="G$1" pin="1" pad="1"/>
<connect gate="G$1" pin="2" pad="2"/>
</connects>
<technologies>
<technology name=""/>
</technologies>
</device>
</devices>
</deviceset>
</devicesets>
</library>
</libraries>
<attributes>
</attributes>
<variantdefs>
</variantdefs>
<classes>
<class number="0" name="default" width="0.6096" drill="0">
</class>
</classes>
<parts>
<part name="ARDUINOMICRO" library="gc1_parts" deviceset="ARDUINO_MICRO" device=""/>
<part name="U$2" library="gc1_parts" deviceset="MICROSD" device=""/>
<part name="U$3" library="gc1_parts" deviceset="MUSCLE_SENSOR" device=""/>
<part name="U$4" library="gc1_parts" deviceset="SWITCH_SS22SDH2" device=""/>
<part name="U$5" library="gc1_parts" deviceset="9V_CONNECTOR" device=""/>
<part name="U$6" library="gc1_parts" deviceset="9V_CONNECTOR" device=""/>
<part name="U$9" library="gc1_parts" deviceset="SWITCH_SS12SDH2" device=""/>
<part name="3.5MMJACK" library="gc1_parts" deviceset="35RAPC2BVN4" device=""/>
<part name="TRIMMER" library="gc1_parts" deviceset="66WR1KLF" device=""/>
<part name="10K_RESISTOR" library="gc1_parts" deviceset="10K_RESISTOR" device=""/>
</parts>
<sheets>
<sheet>
<plain>
</plain>
<instances>
<instance part="ARDUINOMICRO" gate="G$1" x="10.16" y="-41.91" rot="R90"/>
<instance part="U$2" gate="G$1" x="53.34" y="-25.4" rot="R90"/>
<instance part="U$3" gate="G$1" x="-3.81" y="-73.66" rot="R90"/>
<instance part="U$4" gate="G$1" x="55.88" y="-45.72"/>
<instance part="U$5" gate="G$1" x="97.79" y="-43.18"/>
<instance part="U$6" gate="G$1" x="97.79" y="-55.88"/>
<instance part="U$9" gate="G$1" x="29.21" y="-82.55"/>
<instance part="3.5MMJACK" gate="G$1" x="101.6" y="-76.2"/>
<instance part="TRIMMER" gate="G$1" x="58.42" y="-78.74"/>
<instance part="10K_RESISTOR" gate="G$1" x="48.26" y="-99.06"/>
</instances>
<busses>
</busses>
<nets>
<net name="ARDUINO_+5V" class="0">
<segment>
<pinref part="ARDUINOMICRO" gate="G$1" pin="+5V"/>
<wire x1="33.02" y1="-11.43" x2="34.29" y2="-11.43" width="0.1524" layer="91"/>
<label x="35.052" y="-11.938" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="5V"/>
<wire x1="90.17" y1="-21.59" x2="92.71" y2="-21.59" width="0.1524" layer="91"/>
<label x="93.472" y="-22.098" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="10K_RESISTOR" gate="G$1" pin="1"/>
<wire x1="43.18" y1="-99.06" x2="40.64" y2="-99.06" width="0.1524" layer="91"/>
<label x="27.686" y="-99.568" size="1.27" layer="95"/>
</segment>
</net>
<net name="GND" class="0">
<segment>
<pinref part="U$3" gate="G$1" pin="SIG_GND"/>
<wire x1="26.67" y1="-55.88" x2="29.21" y2="-55.88" width="0.1524" layer="91"/>
<label x="30.48" y="-56.388" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="GND"/>
<wire x1="26.67" y1="-68.58" x2="29.21" y2="-68.58" width="0.1524" layer="91"/>
<label x="30.734" y="-69.342" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="GND"/>
<wire x1="90.17" y1="-16.51" x2="92.71" y2="-16.51" width="0.1524" layer="91"/>
<label x="93.726" y="-16.764" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="ARDUINOMICRO" gate="G$1" pin="GND2"/>
<wire x1="33.02" y1="-6.35" x2="34.29" y2="-6.35" width="0.1524" layer="91"/>
<label x="35.052" y="-6.858" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$5" gate="G$1" pin="-"/>
<wire x1="100.33" y1="-43.18" x2="99.06" y2="-43.18" width="0.1524" layer="91"/>
<label x="93.218" y="-43.688" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$6" gate="G$1" pin="+9V"/>
<wire x1="100.33" y1="-50.8" x2="99.06" y2="-50.8" width="0.1524" layer="91"/>
<label x="93.98" y="-51.308" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$9" gate="G$1" pin="1"/>
<wire x1="24.13" y1="-85.09" x2="22.86" y2="-85.09" width="0.1524" layer="91"/>
<label x="18.542" y="-85.598" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="3.5MMJACK" gate="G$1" pin="1"/>
<wire x1="91.44" y1="-71.12" x2="88.9" y2="-71.12" width="0.1524" layer="91"/>
<label x="79.502" y="-70.866" size="1.27" layer="95"/>
</segment>
</net>
<net name="MICROSD_CS" class="0">
<segment>
<pinref part="ARDUINOMICRO" gate="G$1" pin="3/SCL"/>
<wire x1="5.08" y1="-16.51" x2="3.81" y2="-16.51" width="0.1524" layer="91"/>
<label x="-7.62" y="-17.018" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$2" gate="G$1" pin="CS"/>
<wire x1="90.17" y1="-6.35" x2="92.71" y2="-6.35" width="0.1524" layer="91"/>
<label x="93.726" y="-6.858" size="1.27" layer="95"/>
</segment>
</net>
<net name="SENSOR_SIG" class="0">
<segment>
<pinref part="ARDUINOMICRO" gate="G$1" pin="A0"/>
<wire x1="33.02" y1="-31.75" x2="34.29" y2="-31.75" width="0.1524" layer="91"/>
<label x="35.306" y="-32.004" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="SIG"/>
<wire x1="26.67" y1="-58.42" x2="29.21" y2="-58.42" width="0.1524" layer="91"/>
<label x="30.48" y="-58.674" size="1.27" layer="95"/>
</segment>
</net>
<net name="MICROSD_CLK" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="CLK"/>
<wire x1="90.17" y1="-13.97" x2="92.71" y2="-13.97" width="0.1524" layer="91"/>
<label x="93.726" y="-14.478" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="ARDUINOMICRO" gate="G$1" pin="SCK"/>
<wire x1="33.02" y1="1.27" x2="34.29" y2="1.27" width="0.1524" layer="91"/>
<label x="35.052" y="1.016" size="1.27" layer="95"/>
</segment>
</net>
<net name="MICROSD_DO" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="DO"/>
<wire x1="90.17" y1="-11.43" x2="92.71" y2="-11.43" width="0.1524" layer="91"/>
<label x="93.726" y="-11.938" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="ARDUINOMICRO" gate="G$1" pin="MISO"/>
<wire x1="33.02" y1="-1.27" x2="34.29" y2="-1.27" width="0.1524" layer="91"/>
<label x="34.798" y="-1.778" size="1.27" layer="95"/>
</segment>
</net>
<net name="MICROSD_DI" class="0">
<segment>
<pinref part="U$2" gate="G$1" pin="DI"/>
<wire x1="90.17" y1="-8.89" x2="92.71" y2="-8.89" width="0.1524" layer="91"/>
<label x="93.726" y="-9.398" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="ARDUINOMICRO" gate="G$1" pin="MOSI"/>
<wire x1="5.08" y1="1.27" x2="3.81" y2="1.27" width="0.1524" layer="91"/>
<label x="-6.604" y="0.762" size="1.27" layer="95"/>
</segment>
</net>
<net name="+9V_BATT_IN" class="0">
<segment>
<pinref part="U$5" gate="G$1" pin="+9V"/>
<wire x1="100.33" y1="-38.1" x2="99.06" y2="-38.1" width="0.1524" layer="91"/>
<label x="87.63" y="-38.862" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$4" gate="G$1" pin="1"/>
<wire x1="68.58" y1="-48.26" x2="71.12" y2="-48.26" width="0.1524" layer="91"/>
<label x="71.882" y="-48.514" size="1.27" layer="95"/>
</segment>
</net>
<net name="-9V_BATT_IN" class="0">
<segment>
<pinref part="U$6" gate="G$1" pin="-"/>
<wire x1="100.33" y1="-55.88" x2="99.06" y2="-55.88" width="0.1524" layer="91"/>
<label x="86.614" y="-56.642" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$4" gate="G$1" pin="4"/>
<wire x1="50.8" y1="-48.26" x2="48.26" y2="-48.26" width="0.1524" layer="91"/>
<label x="36.576" y="-48.768" size="1.27" layer="95"/>
</segment>
</net>
<net name="-9V_BATT_OUT" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="5"/>
<wire x1="50.8" y1="-50.8" x2="48.26" y2="-50.8" width="0.1524" layer="91"/>
<label x="35.56" y="-51.562" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="-VS"/>
<wire x1="26.67" y1="-66.04" x2="29.21" y2="-66.04" width="0.1524" layer="91"/>
<label x="30.48" y="-66.294" size="1.27" layer="95"/>
</segment>
</net>
<net name="+9V_BATT_OUT" class="0">
<segment>
<pinref part="U$4" gate="G$1" pin="2"/>
<wire x1="68.58" y1="-50.8" x2="71.12" y2="-50.8" width="0.1524" layer="91"/>
<label x="71.882" y="-50.8" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$3" gate="G$1" pin="+VS"/>
<wire x1="26.67" y1="-71.12" x2="29.21" y2="-71.12" width="0.1524" layer="91"/>
<label x="30.226" y="-71.628" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="ARDUINOMICRO" gate="G$1" pin="VIN"/>
<wire x1="33.02" y1="-3.81" x2="34.29" y2="-3.81" width="0.1524" layer="91"/>
<label x="34.544" y="-4.318" size="1.27" layer="95"/>
</segment>
</net>
<net name="SOUND_PIN" class="0">
<segment>
<pinref part="ARDUINOMICRO" gate="G$1" pin="5"/>
<wire x1="5.08" y1="-21.59" x2="3.81" y2="-21.59" width="0.1524" layer="91"/>
<label x="-6.604" y="-22.098" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="TRIMMER" gate="G$1" pin="2"/>
<wire x1="53.34" y1="-78.74" x2="50.8" y2="-78.74" width="0.1524" layer="91"/>
<label x="43.942" y="-78.232" size="1.27" layer="95"/>
</segment>
</net>
<net name="AUDIO_SIGNAL" class="0">
<segment>
<pinref part="3.5MMJACK" gate="G$1" pin="2"/>
<wire x1="91.44" y1="-76.2" x2="88.9" y2="-76.2" width="0.1524" layer="91"/>
<label x="79.756" y="-75.184" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="TRIMMER" gate="G$1" pin="3"/>
<wire x1="53.34" y1="-81.28" x2="50.8" y2="-81.28" width="0.1524" layer="91"/>
<label x="44.704" y="-83.566" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="3.5MMJACK" gate="G$1" pin="4"/>
<wire x1="91.44" y1="-81.28" x2="88.9" y2="-81.28" width="0.1524" layer="91"/>
<label x="79.756" y="-80.518" size="1.27" layer="95"/>
</segment>
</net>
<net name="CONTROL_PIN" class="0">
<segment>
<pinref part="ARDUINOMICRO" gate="G$1" pin="7"/>
<wire x1="5.08" y1="-26.67" x2="3.81" y2="-26.67" width="0.1524" layer="91"/>
<label x="-9.144" y="-27.178" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$9" gate="G$1" pin="2"/>
<wire x1="24.13" y1="-87.63" x2="22.86" y2="-87.63" width="0.1524" layer="91"/>
<label x="9.652" y="-88.138" size="1.27" layer="95"/>
</segment>
</net>
<net name="+5V_RESISTOR" class="0">
<segment>
<pinref part="10K_RESISTOR" gate="G$1" pin="2"/>
<wire x1="43.18" y1="-101.6" x2="40.64" y2="-101.6" width="0.1524" layer="91"/>
<label x="26.924" y="-102.362" size="1.27" layer="95"/>
</segment>
<segment>
<pinref part="U$9" gate="G$1" pin="3"/>
<wire x1="24.13" y1="-90.17" x2="22.86" y2="-90.17" width="0.1524" layer="91"/>
<label x="9.398" y="-90.932" size="1.27" layer="95"/>
</segment>
</net>
</nets>
</sheet>
</sheets>
</schematic>
</drawing>
</eagle>
