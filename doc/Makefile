.PHONY: fullclean clean all doc

all: doc

doc: report.pdf

fullclean: clean
	-rm *.pdf

clean:
	-rm *.aux *.log *.out *.toc

report.pdf: report.tex
	pdflatex report.tex
	pdflatex report.tex
