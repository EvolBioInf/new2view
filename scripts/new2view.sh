./new2view -a ../data/primates.nwk > tmp.out
DIFF=$(diff tmp.out ../data/primates.tex)
if [ "$DIFF" == "" ] 
then
    printf "Test(new2view)\tpass\n"
else
    printf "Test(new2view)\tfail\n"
    echo ${DIFF}
fi

rm tmp.out
