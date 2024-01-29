FLAGS=-Wall -Werror -Wextra
YELLOW=\x1b[33m
WHITE=\x1b[0m

all: s21_grep

s21_grep: grep.o
	gcc $(FLAGS) grep.c -o s21_grep

grep.o: grep.c
	gcc $(FLAGS) -c grep.c

test: grep_test.sh
	bash grep_test.sh

testNoFlags:
	@echo "$(YELLOW)1.Test only file without flags$(WHITE)"
	echo "Hello, World!" > test1.txt
	grep Hello test1.txt > original_output
	./s21_grep Hello test1.txt > custom_output
	diff -s original_output custom_output
	rm test1.txt
	rm original_output
	rm custom_output
	@echo "Done"

testAll: testAll.sh
	bash testAll.sh

testN:
	@echo "$(YELLOW)2. Test flag -n$(WHITE)"
	echo "Line 1\nLine 2\nLine 3" > test2.txt
	grep -n Line test2.txt > original_output
	./s21_grep -n Line test2.txt > custom_output
	diff -s custom_output original_output
	rm test2.txt
	rm original_output
	rm custom_output
	@echo "Done"

testI:
	@echo "$(YELLOW)3. Test flag -i$(WHITE)"
	echo "Line 1\nLine 2\nLine 3" > test3.txt
	grep -i line test3.txt > original_output
	./s21_grep -i line test3.txt > custom_output
	diff -s custom_output original_output
	rm test3.txt
	rm original_output
	rm custom_output
	@echo "Done"
	
testO:
	@echo "$(YELLOW)4. Test flag -o$(WHITE)"
	echo "Line 1\nLine 2\nLine 3" > test4.txt
	grep -o Li test4.txt > original_output
	./s21_grep -o Li test4.txt > custom_output
	diff -s custom_output original_output
	rm test4.txt
	rm original_output
	rm custom_output
	@echo "Done"
	
testH:
	@echo "$(YELLOW)5. Test flag -h$(WHITE)"
	echo "Line 1\nLine 2\nLine 3" > test5.txt
	grep -h Line test5.txt > original_output
	./s21_grep -h Line test5.txt > custom_output
	diff -s custom_output original_output
	rm test5.txt
	rm original_output
	rm custom_output
	@echo "Done"

testF:
	@echo "$(YELLOW)5. Test flag -f$(WHITE)"
	echo "Line\nLine2\nLine3\n1\n2\n3\n" > patfile.txt
	echo "Line 1\nLine 2\nLine 3" > test5.txt
	grep -f patfile.txt test5.txt > original_output
	./s21_grep -f patfile.txt test5.txt > custom_output
	diff -s custom_output original_output
	rm test5.txt
	rm original_output
	rm custom_output
	rm patfile.txt
	@echo "Done"
	
testS:
	@echo "$(YELLOW)6. Test flag -s$(WHITE)"
	echo "Line 1\nLine 2\nLine 3" > test6.txt
	grep -s Line test6.txt > original_output
	./s21_grep -s Line test6.txt > custom_output
	diff -s custom_output original_output
	rm test6.txt
	rm original_output
	rm custom_output
	@echo "Done"
	
testC:
	@echo "$(YELLOW)7. Test flag -c$(WHITE)"
	echo "Line 1\nLine 2\nLine 3" > test7.txt
	grep -c Line test7.txt > original_output
	./s21_grep -c Line test7.txt > custom_output
	diff -s custom_output original_output
	rm test7.txt
	rm original_output
	rm custom_output
	@echo "Done"
	
testV:
	@echo "$(YELLOW)8. Test flag -v$(WHITE)"
	echo "Line 1\nLine 2\nLine 3\nsomething" > test8.txt
	grep -v Line test8.txt > original_output
	./s21_grep -v Line test8.txt > custom_output
	diff -s custom_output original_output
	rm test8.txt
	rm original_output
	rm custom_output
	@echo "Done"
	
testL:
	@echo "$(YELLOW)9. Test flag -l$(WHITE)"
	echo "Line 1\nLine 2\nLine 3" > test9.txt
	grep -l Line test9.txt > original_output
	./s21_grep -l Line test9.txt > custom_output
	diff -s custom_output original_output
	rm test9.txt
	rm original_output
	rm custom_output
	@echo "Done"

clean:
	rm -rf *.o
	rm -rf s21_grep
	rm -rf test.txt
	rm -rf test1.txt
	rm -rf test2.txt
	rm -rf test3.txt
	rm -rf test4.txt
	rm -rf test5.txt
	rm -rf test6.txt
	rm -rf test7.txt
	rm -rf test8.txt
	rm -rf test9.txt
	rm -rf patfile.txt
	rm -rf original_output
	rm -rf custom_output
	rm -rf s21_grep_custom
	rm -rf grep_original
