./new2view > tmp.out
DIFF=$(diff tmp.out ../data/new2view.out)
if [ "$DIFF" == "" ] 
then
    printf "Test(new2view)\tpass\n"
else
    printf "Test(new2view)\tfail\n"
    echo ${DIFF}
fi

rm tmp.out
