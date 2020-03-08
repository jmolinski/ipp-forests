rm *.err; rm *.out
rename ".in" "" *.in

for filename in jm_test*; 
    do cat "$filename" | python py_impl.py 1>"$filename"".out" 2>"$filename"".err"; 
    mv "$filename" "$filename"".in"; 
    printf "$filename \n"; 
done;

