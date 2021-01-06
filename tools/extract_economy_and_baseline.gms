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

# GDP-ADJUSTMENT METRIC
*| PPP | MER |
$setglobal gdp_adjustment 'PPP'

$setglobal baseline 'ssp2'


## CONF
#_____________________________________________________________________________


* Time Discount Rate (ro) of the Ramsey equation
* [0.001, 0.03] with default 0.015 in DICE2016
$setglobal prstp 0.015

* Elasticity of marginal utility of consumption in the Ramsey equation
* with default 1.45 in DICE2016
$setglobal elasmu 1.45

* Capital elasticity in the production function
$setglobal gama 0.300





## FILE SYSTEM SETTINGS
#_____________________________________________________________________________

* Flag that can be used to update paths when launched from external scripts
$setglobal rootfolderpath "../../RICE50x-Auto"

$setglobal modelinputpath "%rootfolderpath%/../../input"
$setglobal datapath       "%rootfolderpath%/data_%region_id%/"
$setglobal inputpath      "%rootfolderpath%/input"
$setglobal temppath       "data_temp/%region_id%"




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


SET
    ssp     'SSP baseline names'       / ssp1,ssp2,ssp3,ssp4,ssp5 /
    gdpadj  'GDP-adjustment type'      / PPP,MER /
;



## PARAMETERS
#_________________________________________________________________________


#MACRO: Load from the same parameter but with first ssp index. Name starts with ssp_
$macro load_from_ssp(par,idx,suxfile) \
parameter ssp_&par(&&idx); \
execute_loaddc '%datapath%data_&suxfile.gdx' ssp_&par; \
&par(&&idx) = ssp_&par(&&idx);


parameter ykali(ssp,t,n) 'GDP for the dynamic calibration [T$]';
load_from_ssp(ykali,'ssp,t,n',baseline)

parameter l(ssp,t,n) 'Population [million people]';
parameter pop(ssp,t,n) 'Population [million people]';
load_from_ssp(l,'ssp,t,n',baseline)

parameter gdppc_kali(ssp,t,n) 'GDP per capita used for calibration [MER]';
gdppc_kali(ssp,t,n) = ykali(ssp,t,n) / l(ssp,t,n) * 1e6;

parameter basegrowthcap(ssp,t,n)          'GDPcap baseline growth factor';


* Adjust population and GDP data until present to historical values maintaining GDP per capita growth
$ifthen.vd not set original_ssp

$gdxin '%datapath%data_validation.gdx'
parameter ykali_valid(t,n), l_valid(t,n);
$loaddc ykali_valid=ykali_valid_wdi l_valid=l_valid_wdi
parameter i_valid(*,t,n);
$loaddc i_valid=i_valid_wdi
$gdxin

parameter lrate(ssp,t,n);
parameter gdppcrate(ssp,t,n);

loop((t,tp1)$(pre(t,tp1)),
  lrate(ssp,tp1,n) = l(ssp,tp1,n) / l(ssp,t,n);
  gdppcrate(ssp,tp1,n) =  gdppc_kali(ssp,tp1,n) / gdppc_kali(ssp,t,n);
);

l(ssp,t,n)$(year(t) le 2015) = l_valid(t,n);
gdppc_kali(ssp,t,n)$(year(t) le 2015) = ykali_valid(t,n) / l_valid(t,n) * 1e6;

loop((t,tp1)$(pre(t,tp1) and (year(tp1) gt 2015)),
  l(ssp,tp1,n) = l(ssp,t,n) * lrate(ssp,tp1,n);
  gdppc_kali(ssp,tp1,n) = gdppc_kali(ssp,t,n) * gdppcrate(ssp,tp1,n);
);

$endif.vd


# Recompute actual quantities used:
parameter ykali_adj(ssp,t,n) ;
ykali_adj(ssp,t,n) = gdppc_kali(ssp,t,n) * l(ssp,t,n) / 1e6;
pop(ssp,t,n) = l(ssp,t,n);

* Conversion factors between PPP and MER
$gdxin '%datapath%data_baseline.gdx'
parameter ppp2mer(t,n) 'ratio to convert PPP to MER';
$loaddc ppp2mer
$gdxin
parameter mer2ppp(t,n) 'ratio to convert MER to PPP';
mer2ppp(t,n) = 1 / ppp2mer(t,n);
#PPP adjustment in case
$if %gdp_adjustment%=="PPP" ykali(ssp,t,n) = ykali(ssp,t,n) * mer2ppp(t,n); ykali_adj(ssp,t,n) = ykali_adj(ssp,t,n) * mer2ppp(t,n); gdppc_kali(ssp,t,n) = gdppc_kali(ssp,t,n) * mer2ppp(t,n);



## STARTING CAPITAL AND SAVINGS RATE
* Take from  calibrated file the capital zero as GDP weighted total capital.
PARAMETERS
    s0(*,t,n)      'Regions Savings Rate at starting time [%GDP]'
    k0(*,t,n)      'Initial Regions Capital at starting time [Trill 2005 USD]'
;

$gdxin '%datapath%data_validation.gdx'
$load k0=k_valid_article, s0=socecon_valid_weo_mean
$gdxin
$if %gdp_adjustment%=="PPP" k0('fg',t,n) = k0('fg',t,n)*mer2ppp(t,n);
s0('savings_rate', '1', n) = s0('savings_rate', '1', n) / 100; #in percent
#use: k0('fg', '1', n) and s0('savings_rate', '1', n)


##  PARAMETERS EVALUATED ----------

PARAMETER
* Starting values
   q0(n)              'Starting (2015) GDP for each region [Trill 2005 USD]'
   e0(n)              'Initial (2015) emissions per each Region [GtCO2-eq]'
* Total factor productivity
   tfp(ssp,t,n)           'Regions Total Factor Productivity'
   i_tfp(ssp,t,n)         'Baselines Investments to evaluate TFP'
   k_tfp(ssp,t,n)         'Baselines Capital to evaluate TFP'

;


PARAMETERS
* Preferences
    elasmu          'Elasticity of marginal utility of consumption'   /  %elasmu%  / # DICE16 1.45 (alpha)
    prstp           'Initial rate of social time preference per year' /  %prstp%   / # DICE16 .015 (ro-discount rate)

* Population and technology
    gama            'Capital elasticity in production function'       /  %gama%    /
    dk              'Depreciation rate on capital (per year)'         /  0.100     /

* Carbon price
    cprice0         'Initial base Carbon Price [2005 USD / tCO2]'     /  2.0       /  #DICE2013: 1.0  #DICE2016: 2.0
    gcprice         'Growth rate of base Carbon Price per year'       /  0.02      /

* Savings Rate
    dice_opt_savings  'Gross Savings Rate level as DICE convergence'   /  0.2751    /  # DICE2016 optim savings convergence!
;


PARAMETERS
* Savings rate
    optlr_savings      'Optimal long-run Savings Rate used for transversality'
    fixed_savings(t,n) 'Gross Savings Rate value if S variable is fixed [%GDP]'

* Carbon price
    scc(t,n)           'Social Cost of Carbon'
    cprice_base(t)     'Carbon Price in base case'
    photel(t,n)        'Carbon Price under no Damages (Hotelling rent condition)'

* Other rates
    rr(t)              'Average utility Social Discount rate'
    ga(t,n)            'Growth rate of Productivity from'
    gl(t,n)            'Growth rate of Labor'


;







PARAMETERS
    ssp_emi_bau(ssp,t,n)   'SSP-Decline rate of decarbonization according to different scenarios (per period)'

    sigma(ssp,t,n)                  'Decline rate of decarbonization (per period)'
    sigma_adj(ssp,t,n)  
;

$gdxin '%datapath%data_baseline_emissions_calibrated.gdx'
$load   ssp_emi_bau=emi_bau_calibrated
$gdxin








##  COMPUTE DATA
#\________________________________________________________________________/



* carbon intensity
sigma(ssp,t,n) = ssp_emi_bau(ssp,t,n)/ykali(ssp,t,n) ;
sigma_adj(ssp,t,n) = ssp_emi_bau(ssp,t,n)/ykali_adj(ssp,t,n) ;

* Starting GDP
q0(n) = ykali('ssp1','1',n) ;

 ##  BASELINE PER-CAPITA GROWTH ------------------------
* Baseline per-capita growth
basegrowthcap(ssp,t,n) = ((( (ykali(ssp,t+1,n)/pop(ssp,t+1,n)) / (ykali(ssp,t,n)/pop(ssp,t,n)) )**(1/tstep)) - 1 )$(t.val < card(t))  ; # last value set to 0

* Carbon price
# cprice_base(t)  = cprice0 * (1+gcprice)**(tstep*(t.val-1));

* Interest Rate
# rr(t)  =  1 / ( (1+prstp)**(tstep*(t.val-1)) );

##  SAVINGS RATE --------

* Optimal long-run Savings rate
optlr_savings = (dk + .004)/(dk + .004*elasmu + prstp)*gama;

* Evaluate converging Savings Rate
* Linear interpolation: S0 + (Send - S0)*(t - t0)/(tend - t0)
fixed_savings(t,n) = s0('savings_rate', '1', n) + (optlr_savings - s0('savings_rate', '1', n)) * (t.val - 1)/(card(t) - 1);



##  DYNAMIC CALIBRATION OF TFP FROM BASELINE AND SCENARIO ---------------------
* set capital first value
k_tfp(ssp,'1',n)  =  k0('fg', '1', n);

* retrieve tfp from reverting Y-I-L process
loop(t,
   # Investments
   i_tfp(ssp,t,n)  =  fixed_savings(t,n)  * ykali(ssp,t,n)   ;
   # Capital
   k_tfp(ssp,t+1,n)  =  ((1-dk)**tstep) * k_tfp(ssp,t,n)  +  tstep * i_tfp(ssp,t,n)  ;
   # TFP of current scenario (explicited from Cobb-Douglas prod. function)
   tfp(ssp,t,n)  =  ykali(ssp,t,n) / ( ( (pop(ssp,t,n)/1000)**(1-gama) )*(k_tfp(ssp,t,n)**gama) )  ;
);


##   OUTPUT
#\____________________________________________________________________________

execute_unload "%temppath%/economy_extracted.gdx" ykali, ykali_adj, pop, tfp, s0, k0, sigma, sigma_adj;
