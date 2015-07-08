# PyramidalLucasKanadeRefined
Test project. Implementation of Lucas Kanade with pyramids for optical flow calculating.

#HOW TO
I implemeted it in QT Creator on the Ubuntu 14.04.

I don't know can it be built on other platforms.

To build this project:

- Open it in QT Creator
- then qmake
- and finaly build project.

After all this steps you can start use it.

Here some parameters:

- smoothing sigma - the smoothing parameter for pyramid gauss smoothing kernel;
- smoothing window size - the size of the smoothing window of the gauss kernel;
- max scaling level - max level for pyramid scaling;
- window size - the size of window of pixel neighbors for Lucas Kanade Algorithm;
- max intensity error - the max allowed error in intensity for calulated;

Color of the resulted optical flow image shows direction of optical flow.
The saturation of the pixel shows optical flow norm.
Black pixels means that calculated flow was bad (big intensity error or out of bounds).

