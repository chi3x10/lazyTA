# lazyTA

## Introduction

**lazyTA** is an app I implemented when I first became TA for a class at University of Florida in 2008. I found it very tedious and mistake-prone to design multiple forms of exam and their corresponding solutions manually. **lazyTA** reads multiple choice questions from a repo file and populates multiple forms of properly formatted latex codes. In particular, lazyTA is capable of the following:

 - randomly chooses questions from repo files. 
 - randomly shuffles the order of the questions. 
 - shuffles the choices of a question. 
 - generates multiple form of latexs from the same selected question. The orders questions and choices in each form are completely different.
 - allows users to specify group questions. Group questions will remain together after the shuffling.
 - allows users to specify questions to be included.

## Repo files. 
A repo file is a text file that contains a collection of multiple choice questions. It must be in the format lazyTA can recognize. Don't worry, it is very similar to HTML. Here are some examples. 

### A simple example
  Just like html files, the basic structure of a repo file
consists of tags. The \<q\> and \<\/q\> tags define a question. Inside these two tags, the \<b\> and
\<\/b\> tags define the body of the question. Each option of the question is enclosed by \<c\> and
\<\/c\> tags. The \<b\> and \<\/b\> tags must always be placed before the \<c\> and \<\/c\> tags.

```
<q>
<b>This is sample question one.</b>
<c> Choice 1.</c>
<c> Choice 2.</c>
<c> Choice 3.</c>
<c> Choice 4.</c>
<c> Choice 5.</c>
<!-- Comment -->
</q>
```

The above question code will generate LaTex codes that can be ”typeset-ed” into the following.
Notice the order of the choices is shuffled.

![Question](https://github.com/chi3x10/lazyTA/blob/feature/fistupload/img/q1.png)

### Qeustions Propertity Tags

**\<ANS\>** This property tag tells the program which choice is the correct one. This is primarily
used to generate the answer key files. When a question doesn’t have an \<ANS\> tag, a
question mark will be placed in the answer key file. Also, in the TA form, the correct answer
will be marked with a box. This tag must be placed in between the \<c\> and \<\/c\> tags.


**\<NOA\>** This tag creates a option ”None of the above.”. This option will always be the last
choice. This tag must be placed in between the \<c\> and \<\/c\> tags.


**\<noshuffle\>** This tag disables the shuffling of choices of a question. It must be placed between the \<q\> and \<\/q\> tags and before the \<b\> tag of the question.


**\<layout=n\>** Force the layout format. n is either 1,2,3, or 4. Without this property tag, the
program automatically determinate layout style of a question based on the length of the
longest option(# of characters). This option allows users to set the layout of the question.
Figure below demonstrates four different layout styles.

![Question](https://github.com/chi3x10/lazyTA/blob/feature/fistupload/img/q2.png)


**\<keepme\>** The question that contains this tag is guaranteed to be selected. It must be placed
between the \<q\> and \<\/q\> tags and before the \<b\> tag of the question.
The following is a more detailed example.

```
<q><keepme><layout=4>
<b>Which state is nicknamed sunshine state?</b>
<c> New York.</c>
<c> Florida.<ANS></c>
<c> <NOA> </c> <!-- None of the above tag -->
<c> Alaska. </c>
<c> Texas.</c>
</q>
```

The latex code generated from the above repo is
![Question](https://github.com/chi3x10/lazyTA/blob/feature/fistupload/img/q3.png)

### Group
Group tag (\<group\> and \<\/group\>) are used when multiple questions must be placed together.
Questions enlosed by \<group\> and \<\/group\> can still be shuffled but only shuffled within the
group. Below is an example of using group tags.

```
<group><noshuffle>

<q><keepme><layout=4>
<b>Which state is nicknamed the sunshine state?</b>
<c> New York.</c>
<c> Florida.<ANS></c>
<c> <NOA> </c> <!-- None of the above tag -->
<c> Alaska. </c>
<c> Teax.</c>
</q>

<q> <b>Continue from previous question. What is the best university of the sunshine state?</b>
<c><NOA></c>
<c>University of Florida. <ANS></c>
<c>NYU.</c>
<c>Teax A\&M. </c>
<c>Collage of survial training. </c> </q>

</group>
```

In the above example, it is very obvious that the 2nd question depends on the first one. Therefore
it’s not appropriate to shuffle the questions within group. The \<noshuffle\> tag right after the
\<group\> tag tells the program not to shuffle the questions within this group. Note: the choices of
each question will still be shuffled unless a \<noshuffle\> tag is inserted inside the question(between \<q\> and \<\/q\>).

### Special tags

**Comments**: Just like html, comments must be placed in between \<!-- and --\>. You can find
examples of comments in the group questions example above.

**\<img\>** and **\<\/img\>**: This allows users to insert images in the exams. You must put the actual latex code. Here is an example:

```
<group>

<img>\includegraphics[bb=0 0 345 423, width=1.4in]{imgs/exam3_fig1.jpg}</img>

<q><b>What is the latin name of the animal in the picture above?</b>
<c>Option1</c><c>Option2</c><c>Option3</c><c>Option4</c><c><NOA></c>
</q>
<q><b>How would you eat it if it’s actually legal to eat it?</b>
<c>Option1</c><c>Option2</c><c>Option3</c><c>Option4</c><c><NOA></c>
</q>

</group>
```

## Running the program

First download the codes using the `git clone` command. Then run
```
mkdir build; cd build;
cmake ..
make
# Run an example.
mkdir ./out
./lazyTA -i ../repos/exam1.rep -tf ../template/template.tex -o out/exam1
```

This will compile the codes and generate the app **lazyTA** in the build folder. Simply run `./lazyTA` in the build folder to see the manual. 

```
-i file_name		read from exam repo "file_name."
-i file_name n		force to read "n" questions from exam repo "file_name."
			Once this number is provided in one file. You have to provide them for other input files too. 
-fp file_name		loads foot print file"file_name"
-o file_name		output filenames in latex format. 
-nf n			generate "n" number of different exams. 
			The output files will be out_filename_form_A, out_filename_B, ..C,..D.....
-nq n			take a total of "n" numbers of questions from the input repo(s).
-tf file_name		use template file (in tex format) "file_name". 
			The default template file is template/template.tex.
-v			turns on verbose mode.
```

Here are some examples:

1. Read repo file exam1.rep. A non-shuffled form (form 0) and a shuffled form (form A)
along with their corresponding TA forms will be generated. Since the output filename is not
specified, the output filenames will be `EXAM_Form_XXX.tex`
```
./lazyTA -i exam1.rep
```

2. Read only 40 questions from repo file exam1.rep and use a template tex file. 

```
./lazyTA -i ../example/exam1.rep 40 -tf ../template/template.tex
```
**Note:** The teamplate tex file is basically the header of a tex file. Without a proper
template file, `lazyTA` will generate tex codes that cannot be compile by latex. 

3. Read a total of 50 questions from repo file exam1 .rep and exam1 2.rep.
```
./lazyTA -i exam1_1.rep -i exam1_2.rep -nq 50
```

Numbers of questions selected from each repo will be determined proportionally based
on the total number of questions in each repo.

4. Read 10 questions from exam1 .rep and 25 questions from exam1 2.rep.
```
./lazyTA -i exam1_1.rep 10 -i exam1_2.rep 25
```

5. Generate 3 different shuffled forms. The filenames will be EXAM Form A/B/C...tex.
```
./lazyTA -i exam1_1.rep -nf 3
```

6. Read from exam1.rep and the output filename will be midterm Form A.tex.
```
./lazyTA -i exam1_1.rep -o midterm
```
## Useful Scripts

- `pdf_all_tex_files.sh`: Compile all tex files in a folder to pdfs.
- `bubble.m`: A matlab script to fill in the solution on a scantron form. Unless you are using the exact same scantron as we did, you will need to modify it. 

## License
**lazyTA** is released under a [GPLv3 license](https://choosealicense.com/licenses/gpl-3.0/).
