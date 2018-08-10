#!/bin/bash
rm *.pdf
for x in *.tex; 
    do pdflatex $x; 
done
rm *.aux
rm *.log
rm *.dvi

#
# for x in *.pdf;
#     do pdftk A=$x B=forms/exam3_ss_fa10.pdf C=forms/blank.pdf D=forms/end_page.pdf cat A B C D output r_$x
# done
