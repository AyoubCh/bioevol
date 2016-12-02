make 
valgrind --tool=callgrind --callgrind-out-file=out.callgrind ./pdc_evol_model -t
kcachegrind  out.callgrind &

