* baseline data
$offdigit
$offend
$onmulti
$onempty
$setenv gdxcompress 1
$onrecurse
$eolcom #



$setglobal region_id "norsou2"
$setglobal time_id   "t58"





## FILE SYSTEM SETTINGS
#_____________________________________________________________________________

* Flag that can be used to update paths when launched from external scripts
$setglobal rootfolderpath "../../RICE50x-Auto"

$setglobal modelinputpath "%rootfolderpath%/../../input"
$setglobal datapath       "%rootfolderpath%/data_%region_id%/"
$setglobal inputpath      "%rootfolderpath%/input"
$setglobal temppath       "../data/temp"




##  SETS
#_________________________________________________________________________

SET n 'Regions' /
$batinclude "%datapath%n.inc"
/;

SETS
    t                 'Time period nodes'  # values in time.inc included below
* Time control
    tfirst(t)         'First time period nodes'
    tsecond(t)        'Second time period node'
    tlast(t)          'Last time period nodes'
    last10(t)         'Last 10 period nodes'
;
alias(t,tt,ttt,tp1,tp2,tm1,tm2);

SET       pre(t,tp1)  'Precedence set, t is the predecessor of tp1'    ;
SET       preds(t,tt) 'Predecessors set, tt are all predecessors of t' ;
PARAMETER tperiod(t)  'Time period'                                    ;
PARAMETER year(t)     'Reference year for period t'                    ;
PARAMETER begyear(t)  'Beginning year for period t'                    ;
PARAMETER tlen(t)     'Length of time period [years]'                  ;
SCALAR    tstep       'Length of each time step [years]'       / 5 /   ;

$include '%datapath%/time.inc'


set yearlu /1850*2300/;


## PARAMETERS
#_________________________________________________________________________


PARAMETERS
** LU-Baseline
   eland0(n)        'Carbon emissions from land in 2015 [GtCO2/year]'
   deland           'Decline rate of land emissions (per period)'         / .115  /
   eland_bau(*,t,n) 'Carbon emissions baselines from land [GtCO2/year]'

   CtoCO2
   CO2toC
;

* Historical EMISSIONS
PARAMETER q_emi_valid_primap(*,yearlu,n)  'Historical Emissions per each region [GtC]';
$gdxin  '%datapath%data_historical_values.gdx'
$load   q_emi_valid_primap
$gdxin


##  COMPUTE DATA
#_________________________________________________________________________

CtoCO2 = 44 / 12 ;
CO2toC = 12 / 44 ;

* starting value averaged over last 10 year ( to minimize risk of high fluctuations)
eland0(n)  = CtoCO2 * sum(yearlu, q_emi_valid_primap('co2lu',yearlu,n)$((yearlu.val ge 2005) and (yearlu.val lt 2015))) / 10  ;

loop(t,
* UNIFORM LOGIC
    eland_bau('uniform',t,n)  =  eland0(n)*(1-deland)**(t.val-1) ;
* DIFFERENTIATED
    # if negative, it remains fixed to its value
    # otherwise smmoth decreasing at DICE2016 rate.
    eland_bau('differentiated',t,n)  =  min(eland0(n)*(1-deland)**(t.val-1), eland0(n) ) ;
);

* global values
PARAMETER global_eland_bau(*,t);
  global_eland_bau('uniform',t)  = sum(n, eland_bau('uniform',t,n))    ;
  global_eland_bau('differentiated',t)  = sum(n, eland_bau('differentiated',t,n))   ;

PARAMETER cumeland_bau(*,t,n), global_cumeland_bau(*,t);
cumeland_bau('uniform','1',n)= 0;
cumeland_bau('differentiated','1',n)= 0;

loop(t,
 cumeland_bau('uniform', t+1, n) =  cumeland_bau('uniform', t, n) + eland_bau('uniform',t,n);
 cumeland_bau('differentiated', t+1, n) =  cumeland_bau('differentiated', t, n) + eland_bau('differentiated',t,n);
);

global_cumeland_bau('uniform',t) = sum(n, cumeland_bau('uniform',t,n)) ;
global_cumeland_bau('differentiated',t) = sum(n, cumeland_bau('differentiated',t,n)) ;



##   OUTPUT
#\____________________________________________________________________________

PARAMETER etree_bau(t,n), etree_opt(t,n);

etree_bau(t,n) = eland_bau('uniform',t,n)  ;
etree_opt(t,n) =  eland_bau('differentiated',t,n)  ;

execute_unload "%temppath%/land_use_extracted.gdx" etree_bau, etree_opt;
