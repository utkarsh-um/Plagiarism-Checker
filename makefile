default:plagChecker

plagChecker:plagiarism.c
	@gcc plagiarism.c -lm -o plagChecker
.Phony : plagChecker