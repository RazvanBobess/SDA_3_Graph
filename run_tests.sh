#!/bin/bash

print_header()
{
	header="${1}"
	header_len=${#header}
	printf "\n"
	if [ $header_len -lt 71 ]; then
		padding=$(((71 - $header_len) / 2))
		for ((i = 0; i < $padding; i++)); do
			printf " "
		done
	fi
	printf "= %s =\n\n" "${header}"
}

result1=0
result2=0
result=0
ZERO=0
BONUSVALGRIND=0
README=0
VALGRIND="valgrind --leak-check=full --track-origins=yes -q --log-file=rezultat_valgrind.txt"

print_header "Tema 3"

make build > /dev/null 2>&1

#Cerința 1 
echo "================================= CERINTA 1 =================================="
for i in {0..9}
do
	fileIn="input/cerinta1/test"$i".in"
	fileRef="ref/cerinta1/test"$i".ref"
	fileOut="tema3.out"
    fileOutDirectory="output/cerinta1/test"$i".out"
	cp "$fileIn" "tema3.in"
        rm $fileOut > /dev/null 2>&1
	timeout 20 ./tema3 1 > /dev/null 2>&1
    cp $fileOut $fileOutDirectory
	diff $fileOut $fileRef -w > /dev/null
	EXIT_CODE=$?
	idx=$i
	if [ $EXIT_CODE -eq $ZERO ] 
	then
		echo "+4.5: Cerința 1 Test $idx .............................................. PASS"
		result=$(echo "$result + 4.5" | bc -l)
		result1=$(echo "$result1 + 4.5" | bc -l)
		$VALGRIND ./tema3 1 > /dev/null 2>&1
		if [[ ! -z $(cat rezultat_valgrind.txt) ]]; then
			echo "0.0: VALGRIND Cerinta 1 Test $i .......................................... FAIL"
		else
			BONUSVALGRIND=$(echo "$BONUSVALGRIND + 1.0" | bc -l)
			printf "+1.0: VALGRIND Cerinta 1 Test $i ......................................... PASS\n"
		fi
	else
		echo "0.0: Cerința 1 Test $idx .............................................. FAIL"
	fi
done

rm "tema3.in"
rm "tema3.out"

#Cerința 2 
echo "================================= CERINTA 2 =================================="
for i in {0..9}
do
	fileIn="input/cerinta2/test"$i".in"
	fileRef="ref/cerinta2/test"$i".ref"
	fileOut="tema3.out"
	fileOutDirectory="output/cerinta2/test"$i".out"
	cp "$fileIn" "tema3.in"
        rm $fileOut > /dev/null 2>&1
	timeout 20 ./tema3 2 > /dev/null 2>&1
	cp $fileOut $fileOutDirectory
	diff $fileOut $fileRef -w > /dev/null
	EXIT_CODE=$?
	idx=$i
	if [ $EXIT_CODE -eq $ZERO ] 
	then
		echo "+5.0: Cerința 2 Test $idx ................................................. PASS"
		result=$(echo "$result + 5.0" | bc -l)
		result2=$(echo "$result2 + 5.0" | bc -l)
		$VALGRIND ./tema3 2 > /dev/null 2>&1
		if [[ ! -z $(cat rezultat_valgrind.txt) ]]; then
			echo "0.0: VALGRIND Cerinta 2 Test $i .......................................... FAIL"
		else
			BONUSVALGRIND=$(echo "$BONUSVALGRIND + 1.0" | bc -l)
			printf "+1.0: VALGRIND Cerinta 2 Test $i ......................................... PASS\n"
		fi
	else
		echo "0.0: Cerința 2 Test $idx ................................................. FAIL"
	fi
done

rm "tema3.in"
rm "tema3.out"

echo "=================================== README ==================================="
if test -f "README"; then
	echo "+5.0: Verificare README ................................................. PASS"
	README=$(echo "$README + 5.0" | bc -l)
else
	echo "0.0: Verificare README ................................................ FAIL"
fi

result=$(echo "$result + $BONUSVALGRIND" | bc -l)
result=$(echo "$result + $README" | bc -l)

echo "================================= REZULTATE =================================="
printf "Cerința 1 : "$result1/45p"\n"
printf "Cerința 2 : "$result2/50p"\n"
printf "README : "$README"/5p\n"
printf "BONUS VALGRIND : "$BONUSVALGRIND"/20p\n"
printf "Punctajul Acumulat este: "$result"/120p!\n"
make clean > /dev/null 2>&1
