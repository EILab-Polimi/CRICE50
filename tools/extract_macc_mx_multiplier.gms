* baseline data
$offdigit
$offend
$onmulti
$onempty
$eolcom #



$setglobal region_id "norsou2"



## FILE SYSTEM SETTINGS
#_____________________________________________________________________________

* Flag that can be used to update paths when launched from external scripts
$setglobal rootfolderpath "../../RICE50x-Auto"

$setglobal datapath       "%rootfolderpath%/data_%region_id%"
$setglobal inputpath      "%rootfolderpath%/input"
$setglobal temppath       "../data/temp"




## CONF
#_____________________________________________________________________________


# CORRECTION MULTIPLIER
* | ssp2marker | advance | ssp2markerXT | advanceXT |
$setglobal mxdataref ssp2marker


# BACKSTOP DATA
* (DICE2016 as default)
$setglobal pback      550
$setglobal gback      0.025
$setglobal expcost2   2.8
* time starting transition to pbackstop
$setglobal tstart_pbtransition   7
* time of full-convergence to backstop curve [18,38]
$setglobal tend_pbtransition   23
* parameter influencing logistic transition speed (0,2]
$setglobal klogistic 0.25

# MACC SHAPE
* MACC fitting model | cstay14fit [polinomial 1-4] |
$setglobal maccfit "poly14fit"



## CONF
#_____________________________________________________________________________

SET n 'Regions' /
$batinclude "%datapath%/n.inc"
/;

SET t 'Time' /1*58/;

SETS

   coef    'fit coefficients for MACC'  / a, b /

   sector  'EnerData sectors'   /
        Total_CO2
        Total_CH4
        #Total_HFC
        Total_N2O
        #Total_PFC
        #Total_SF6
    /


;

* MX alternative data references
SET mxdataref 'Rata-reference for macc multiplier calibration'/

        ssp2marker
        advance
        ssp2markerXT
        advanceXT
    /
;


## PARAMS
#_____________________________________________________________________________


##  PARAMETERS HARDCODED OR ASSIGNED -------------------

PARAMETERS
* DICE backstop
    expcost2  "Exponent of control cost function"                / %expcost2% / #DICE: 2.8
    pback     "Cost of backstop 2010$ per tCO2 in 2015"          / %pback%    / #DICE2013: 344     #DICE2016: 550
    gback     "Initial cost decline backstop cost per period"    / %gback%    / #DICE2013: 0.05    #DICE2016: 0.025
;


##  PARAMETERS LOADED ----------------------------------

* Correction multiplier calibrated
PARAMETER  MXkali(mxdataref)  "Correction multiplier calibrated over enerdata times"  ;
$gdxin '%datapath%/data_mod_macc_correcting_factor.gdx'
$load  MXkali
$gdxin


* CO2 MAC-Curves fitting parameters
PARAMETER  macc_%maccfit%_enerdata_CO2(sector,coef,t,n)  'EnerData CO2 MACC -fit with %maccfit%- for given years (2025-2040)'  ;
$gdxin '%datapath%/data_macc_enerdata_co2perc_fit.gdx'
$load  macc_%maccfit%_enerdata_CO2    = abat_coef_enerdata_%maccfit%
$gdxin



PARAMETERS
* Backstop
    pbacktime(t)            "Backstop price"
    cost1(t,n)              "Adjusted cost for Backstop"
    partfract(t)            "Fraction of emissions in control regime"
* MACC fit coefficients
    ax_co2(*,sector,t,n)    "EnerData < a > %maccfit%-coeff for MACC"
    bx_co2(*,sector,t,n)    "EnerData < b > %maccfit%-coeff for MACC"
    a_oghg(sector,t,n)      "EnerData < a > fit-coeff for MACC"
    b_oghg(sector,t,n)      "EnerData < b > fit-coeff for MACC"
* MACC transition
    mx(t,n)                 "Enerdata MACC multiplier calibated on diagnostics"
    alpha(t)                "Transition to backstop coefficient"
    MXpback(t,n)            "MX to obtain full pbackstop"
    MXstart(n)              "MX starting value"
    MXend(t,n)              "MX value to be reached"
    MXdiff(t,n)             "MX transition gap"
;


##  COMPUTE DATA
#_________________________________________________________________________



#  PBackstop curve -------------------------------------

pbacktime(t)  =  pback*(1-gback)**(t.val-1);
# cost1(t,n)    =  pbacktime(t)*sigma(t,n)/expcost2/1000;

ax_co2('%maccfit%','Total_CO2',t,n)    = macc_%maccfit%_enerdata_CO2('Total_CO2', 'a', t, n);
bx_co2('%maccfit%','Total_CO2',t,n)    = macc_%maccfit%_enerdata_CO2('Total_CO2', 'b', t, n);


# TRANSITION TO BACKSTOP -------------------------------
* It is directly related to settings from conf phase.
* Shape, slope and convergence time are all taken into account here

## logistic pbtransition
scalar x0 ;
x0 = %tstart_pbtransition% + ((%tend_pbtransition%-%tstart_pbtransition%)/2)  ;
alpha(t) = 1/(1+exp(-%klogistic%*(t.val-x0)));


MXpback(t,n)  =  pbacktime(t) /  ((ax_co2('%maccfit%','Total_CO2',t,n) * 1)  +  (bx_co2('%maccfit%','Total_CO2',t,n) * power(1,4)))  ;


# NOTE ........................................................................
# Before transition the original multiplier applies,
# then a smooth  transition to the pback-curve is performed,
# by progressively reducing the distance (according to the shaping alpha-param)
#...............................................................................

MXstart(n)   =  MXkali('%mxdataref%')  ;
MXend(t,n)   =  MXpback(t,n)  ;
MXdiff(t,n)  =  max( MXstart(n)  - MXend(t,n),0) ;

* Final coefficient values:
mx(t,n)      =  MXstart(n)  -  alpha(t) * MXdiff(t,n);





##   OUTPUT
#\____________________________________________________________________________

execute_unload "%temppath%/mx_multiplier.gdx" mx;
