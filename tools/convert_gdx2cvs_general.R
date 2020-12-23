# CRICE50x: CONVERT GDX 2 CSV 
# ---------------------------------------------------------------- #
# Transform Gams datafile into CSV equivalent
# ---------------------------------------------------------------- #
#

#  .rs.restartR()        # only if you want to restart R
rm(list = ls())       # oly if you want to clean environment



## ======  Packages  =================


library("witchtools")
library("rice50xplots")

pkgs <- c('ggplot2',
          'tidyr',
          'broom',
          'data.table',
          'docopt',
          'stringr',
          'dplyr',
          'dtplyr',
          'purrr', 
          'colf', 
          'xlsx',
          'gdxtools', 
          'here',
          'gridExtra',
          'grid',
          'wesanderson',
          'latex2exp',
          "minpack.lm", 
          "crayon")
res <- suppressWarnings(lapply(pkgs,require_package))
suppressWarnings(require_gdxtools())


'%_%' <- function(a,b) paste0(a,b)




## ====== INPUT/OUTPUT ===========



# INPUT ......................

# in_riceproject_dir <- "../RICEx-Main-Exp/RICExdata_ed57_t58"
in_riceproject_dir <- "../RICE50x-Auto/data_norsou2"



# in_filename <- "RICExdata_mod_climate_WITCHcoop.gdx"
in_filename <- "data_baseline.gdx"



# OUTPUT ......................

out_dirname = "data_baseline"




## ====== SOURCE GDX =============



mygdx = gdx(here(in_riceproject_dir%_%"/"%_%in_filename))

mygdx$parameters






## ===== OUTPUT DESTINATION ==========


# create dir
output_dir = here("data/"%_%out_dirname%_%"/")
dir.create(here("data"))
dir.create(output_dir)





## ===== EXTRACT + CONVERT+ SAVE  =========

for(par in mygdx$parameters$name){
  
  # Test:   par = mygdx$parameters$name[1]
  
  mydf = mygdx[par]
  write.csv(x = mydf, file = output_dir%_%"/"%_%par%_%".csv")
  
}
