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




## ====== GENERAL SETTINGS =======

setwd((here()))

# in_riceproject_dir <- "../RICEx-Main-Exp/RICExdata_ed57_t58"
in_riceproject_dir <- "../RICE50x-Auto/data_norsou2"
in_temp_dir <- "data/temp"
dir.create(here("data"))




#------------------------------------------------------#
####      _______ GENERAL CONVERSIONS ________      ####
#------------------------------------------------------#





# ►   BASELINE GDP/POP                                      ####
#__________________________________________________________



in_filename <- "data_baseline.gdx"
out_dirname <- "data_baseline"

# create output dir
output_dir = here("data/"%_%out_dirname%_%"/")
dir.create(output_dir)
temp_out = list()


# Source gdx
mygdx = gdx(here(in_riceproject_dir%_%"/"%_%in_filename))
mygdx$parameters



# ==== params =======

# ykali 
param <- "ssp_ykali"
temp_out[[param]] <- mygdx[param] %>% mutate(t = as.numeric(t)) %>% rename(ssp = V1) %>% arrange(ssp,t,n) %>% mutate(ssp = tolower(ssp))


# pop 
param <- "ssp_l"
temp_out[["ssp_pop"]] <- mygdx["ssp_l"] %>% mutate(t = as.numeric(t)) %>% rename(ssp = V1)  %>% arrange(ssp,t,n) %>% mutate(ssp = tolower(ssp))



# ppp2mer
param <- "ppp2mer"
temp_out[[param]]  <- mygdx[param] %>% mutate(t = as.numeric(t))  %>% arrange(t,n)



# ===  write csv  ========

for(par in names(temp_out)){
  
  # Test:   par = mygdx$parameters$name[1]
  
  mydf = temp_out[[par]]
  write.csv(x = mydf, file = output_dir%_%"/"%_%par%_%".csv", row.names=FALSE)
  
}





# ►   BASELINE EIND                                    ####
#__________________________________________________________


in_filename <- "data_baseline_emissions_calibrated.gdx"
out_dirname <- "data_baseline"

# create output dir
output_dir = here("data/"%_%out_dirname%_%"/")
dir.create(output_dir)
temp_out = list()


# Source gdx
mygdx = gdx(here(in_riceproject_dir%_%"/"%_%in_filename))
mygdx$parameters




# ==== params =======


# eind 
temp_out[["ssp_eind_bau"]] <- mygdx["emi_bau_calibrated"] %>% mutate(t = as.numeric(t)) %>% rename(ssp = V1)  %>% arrange(ssp,t,n)



# ====  write csv  ========

for(par in names(temp_out)){
  
  # Test:   par = mygdx$parameters$name[1]
  
  mydf = temp_out[[par]]
  write.csv(x = mydf, file = output_dir%_%"/"%_%par%_%".csv", row.names=FALSE)
  
}








# ►   BASELINE EXTRACTED                              ####
#__________________________________________________________


in_filename <-  "economy_extracted.gdx"
out_dirname <- "data_baseline"

# create output dir
output_dir = here("data/"%_%out_dirname%_%"/")
dir.create(output_dir)
temp_out = list()


# Source gdx
mygdx = gdx(here(in_temp_dir%_%"/"%_%in_filename))
mygdx$parameters



# ====  params  ========

temp_out[["ssp_ykali_adj"]] <- mygdx["ykali_adj"]  %>% mutate(t = as.numeric(t)) %>% arrange(ssp,t,n)


temp_out[["ssp_tfp"]] <- mygdx["tfp"]  %>% mutate(t = as.numeric(t)) %>% arrange(ssp,t,n)


temp_out[["ssp_cintensity"]] <- mygdx["sigma"]  %>% mutate(t = as.numeric(t)) %>% arrange(ssp,t,n)


temp_out[["ssp_cintensity_adj"]] <- mygdx["sigma_adj"]  %>% mutate(t = as.numeric(t)) %>% arrange(ssp,t,n)


# ===  write csv  ========

for(par in names(temp_out)){
  
  # Test:   par = mygdx$parameters$name[1]
  
  mydf = temp_out[[par]]
  write.csv(x = mydf, file = output_dir%_%"/"%_%par%_%".csv", row.names=FALSE)
  
}









# ►   ECONOMY EXTRACTED                              ####
#__________________________________________________________


in_filename <-  "economy_extracted.gdx"
out_dirname <-  "data_economy"

# create output dir
output_dir = here("data/"%_%out_dirname%_%"/")
dir.create(output_dir)
temp_out = list()


# Source gdx
mygdx = gdx(here(in_temp_dir%_%"/"%_%in_filename))
mygdx$parameters



# ====  params  ========

temp_out[["s0"]] <- mygdx["s0"]  %>% filter(V1=="savings_rate", t=="1") %>% select(n,value)


temp_out[["k0"]] <- mygdx["k0"]  %>% select(n,value)



# ===  write csv  ========

for(par in names(temp_out)){
  
  # Test:   par = mygdx$parameters$name[1]
  
  mydf = temp_out[[par]]
  write.csv(x = mydf, file = output_dir%_%"/"%_%par%_%".csv", row.names=FALSE)
  
}









# ►   LAND-USE EXTRACTED                              ####
#__________________________________________________________


in_filename <-  "land_use_extracted.gdx"
out_dirname <-  "data_land_use"

# create output dir
output_dir = here("data/"%_%out_dirname%_%"/")
dir.create(output_dir)
temp_out = list()


# Source gdx
mygdx = gdx(here(in_temp_dir%_%"/"%_%in_filename))
mygdx$parameters



# ====  params  ========

temp_out[["etree_bau"]] <- mygdx["etree_bau"]   %>% mutate(t = as.numeric(t)) %>% arrange(t,n)


temp_out[["etree_opt"]] <- mygdx["etree_opt"]   %>% mutate(t = as.numeric(t)) %>% arrange(t,n)



# ===  write csv  ========

for(par in names(temp_out)){
  
  # Test:   par = mygdx$parameters$name[1]
  
  mydf = temp_out[[par]]
  write.csv(x = mydf, file = output_dir%_%"/"%_%par%_%".csv", row.names=FALSE)
  
}








# ►   CLIMATE WITCH                                    ####
#__________________________________________________________


in_filename <- "data_mod_climate.gdx"
out_dirname <- "data_climate_witch"

# create output dir
output_dir = here("data/"%_%out_dirname%_%"/")
dir.create(output_dir)
temp_out = list()


# Source gdx
mygdx = gdx(here(in_riceproject_dir%_%"/"%_%in_filename))
mygdx$parameters



# ===  params  ========

 
param <- "cceq"
temp_out[[param]] <- mygdx[param] %>% rename(layer = V1)

param <- "cmdec1"
temp_out[[param]]  <- mygdx[param] %>% rename(oghg = V1)

param <- "cmdec2"
temp_out[[param]] <- mygdx[param] %>% rename(oghg = V1)

param <- "cmeq"
temp_out[[param]] <- mygdx[param] %>% rename(layer = V1)

param <- "cmphi"
temp_out[[param]] <- mygdx[param] %>% rename(layer1 = V1, layer2 = V2)

param <- "emi_gwp"
temp_out[[param]] <- mygdx[param] %>% rename(ghg = V1)

param <- "emi_preind"
temp_out[[param]] <- mygdx[param] %>% rename(oghg = V1)

param <- "rfaerosols"
temp_out[[param]]  <- mygdx[param]  %>% mutate(t = as.numeric(t)) %>% arrange(t)

param <- "rfc"
temp_out[[param]]  <- mygdx[param] %>% rename(ghg = V1, "param" = V2)

param <- "tempc"
temp_out[[param]]  <- mygdx[param] %>% rename("param" = V1)

param <- "wcum_emi_eq"
temp_out[[param]]  <- mygdx[param] %>% rename(oghg = V1)

param <- "wcum_emi0"
temp_out[[param]]  <- mygdx[param] %>% rename(ghg = V1)



# ===  write csv  ========

for(par in names(temp_out)){
  
  # Test:   par = mygdx$parameters$name[1]
  
  mydf = temp_out[[par]]
  write.csv(x = mydf, file = output_dir%_%"/"%_%par%_%".csv", row.names=FALSE)
  
}










# ►   CLIMATE OGHG                                    ####
#__________________________________________________________


in_filename <- "data_ssp_iam.gdx"
out_dirname <- "data_climate_witch"

# create output dir
output_dir = here("data/"%_%out_dirname%_%"/")
dir.create(output_dir)
temp_out = list()


# Source gdx
mygdx = gdx(here(in_riceproject_dir%_%"/"%_%in_filename))
mygdx$parameters



# ===  params  ========


param <- "oghg_coeff"
temp_out[[param]] <- mygdx[param] %>% rename(coeff = V1)



# ===  write csv  ========

for(par in names(temp_out)){
  
  # Test:   par = mygdx$parameters$name[1]
  
  mydf = temp_out[[par]]
  write.csv(x = mydf, file = output_dir%_%"/"%_%par%_%".csv", row.names=FALSE)
  
}






# ►   CLIMATE REGIONAL                                    ####
#__________________________________________________________


in_filename <- "data_mod_climate_regional.gdx"
out_dirname <- "data_climate_regional"

# create output dir
output_dir = here("data/"%_%out_dirname%_%"/")
dir.create(output_dir)
temp_out = list()


# Source gdx
mygdx = gdx(here(in_riceproject_dir%_%"/"%_%in_filename))
mygdx$parameters



# ===  params  ========


param <- "climate_region_coef"
temp_out[[param]] <- mygdx[param] %>% rename(coef = V1)




# ===  write csv  ========

for(par in names(temp_out)){
  
  # Test:   par = mygdx$parameters$name[1]
  
  mydf = temp_out[[par]]
  write.csv(x = mydf, file = output_dir%_%"/"%_%par%_%".csv", row.names=FALSE)
  
}






# ►   MACC COEFFS                                    ####
#__________________________________________________________



in_filename <- "data_macc_enerdata_co2perc_fit.gdx"
out_dirname <- "data_macc"

# create output dir
output_dir = here("data/"%_%out_dirname%_%"/")
dir.create(output_dir)
temp_out = list()


# Source gdx
mygdx = gdx(here(in_riceproject_dir%_%"/"%_%in_filename))
mygdx$parameters



# ====  params  ========

temp_out[["macc_coeffs"]] <- mygdx["abat_coef_enerdata_poly14fit"] %>%  filter(sector == "Total_CO2") %>% select(-sector) %>%  mutate(t = as.numeric(t)) %>% select(coef,t,n,value) %>% arrange(coef,t,n) 



# ===  write csv  ========

for(par in names(temp_out)){
  
  # Test:   par = mygdx$parameters$name[1]
  
  mydf = temp_out[[par]]
  write.csv(x = mydf, file = output_dir%_%"/"%_%par%_%".csv", row.names=FALSE)
  
}






# ►   MACC MX MULTIPLIER                               ####
#__________________________________________________________


in_filename <-  "mx_multiplier.gdx"
out_dirname <- "data_macc"

# create output dir
output_dir = here("data/"%_%out_dirname%_%"/")
dir.create(output_dir)
temp_out = list()


# Source gdx
mygdx = gdx(here(in_temp_dir%_%"/"%_%in_filename))
mygdx$parameters



# ====  params  ========

temp_out[["mx_multiplier"]] <- mygdx["mx"] %>%   mutate(t = as.numeric(t)) %>% arrange(t,n) 




# ===  write csv  ========

for(par in names(temp_out)){
  
  # Test:   par = mygdx$parameters$name[1]
  
  mydf = temp_out[[par]]
  write.csv(x = mydf, file = output_dir%_%"/"%_%par%_%".csv", row.names=FALSE)
  
}



