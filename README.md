# Edge Detection

This work was initially developed during the Computational Vision discipline of Professor Aldo von Wangenheim at the Postgraduate Program in Computer Science (PPGCC) at the Federal University of Santa Catarina (UFSC) in the second semester of 2001. In the discipline I implemented a program that performed the detection of edges in an image using an Artificial Neural Network with Backpropagation learning.

Link of work in the discipline: [https://www.inf.ufsc.br/~aldo.vw/visao/2001/saulo/]

Later, in 2005, during my doctorate in the Post-Graduate Program in Automation and Systems Engineering (PPGEAS) at the same university, I suggested another algorithm for edge detection that used only conditionals (single and double). I wrote an article with the help of my doctoral colleague Rui Jorge Tramontin entitled "Proposal for Optimization of Edge Detection Algorithms in Images" which was accepted as a Poster for presentation at the "4th International Information and Telecommunication Technologies Symposium" in 2005.

![Edge Detection](doc/edge_detection.png)

For the article, I implemented the Roberts (and variant), Sobel (and variant) edge detection techniques, I used the edge detection by Neural Network that I had already implemented in Professor Aldo's course, I has implemented the algorithm using only one perceptron and the algorithms and I proposed of Single Conditional and Double Conditional. For the article, a visual and execution time comparison of the presented algorithms was made.

The paper and poster presented are in the doc folder.

## Execution Runtime Comparison

The results presented here are from the time of writing the article.

### Runtime Environment

* Computer Pentium IV 3GHz,  RAM 1GB
* Operationg System Linux
* Programming Language C
* Original image resolution 2048x2560 pixels
* Execution time result at: October 05, 2005

### Runtime Results

| Algorithm  |   1o  |   2o  |   3o  | Average|
| :--------- | ----: | ----: | ----: | -----: |
| Roberts    |  0.60 |  0.60 |  0.60 |  0.60  |
| RobertsAbs |  0.10 |  0.09 |  0.10 |  0.09  |
| Sobel      |  0.64 |  0.64 |  0.65 |  0.64  |
| SobelAbs   |  0.15 |  0.15 |  0.16 |  0.15  |
| Backprop   | 19.39 | 19.49 | 19.48 | 19.48  |
| Perceptron |  1.05 |  1.06 |  1.05 |  1.05  |
| If         |  0.06 |  0.06 |  0.06 |  0.06  |
| Double If  |  0.08 |  0.08 |  0.08 |  0.08  |

## Considerations and Modifications

* The codes were developed in C using allegro library and could only work with BMP images. To publish to github, I made some recent changes (June 10, 2021).
* To use a Multilayer Perceptron with Backpropagation, at the time I used QwikNet V2.23 software that generates code in C to run the network. As much as I have my own implementations of Artificial Neural Networks Multilayer Perceptrons today, for this program I keep the same resource used at the time.
* I switched to Simple Direct Layer 2 (SDL2) library to be able to work with other types of images;
* Each filter was coded in a separate program and a lot of code was rewritten. I put all filters in a single program, selecting the filter by parameter via the command line.
* I changed the photo from the time to a current one and "much prettier" to be used in the tests.
