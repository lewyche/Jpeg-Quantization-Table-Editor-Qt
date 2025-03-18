![image](https://github.com/user-attachments/assets/3328ef4d-c679-46d2-a9ac-92a328adc651)

### A Jpeg Quantization Table Editor
This program allows you to edit the Quantization tables of a Jpeg image, as well as well as adjusting its quality.
The primary uses for this program include:
 - Data moshing an image.
 - Learning how a quantization table works.
 - Adjusting quality of an image.

### Tutorial
- Press the file button on the top left, and then press import.
- Import an image.
- Start editing the image!
  - The arrow buttons switch between quantization tables.
  - The text field must always have 64 valid hexadecimal numbers, the program will indicate whether the input is valid.
  - Always press save after making edits to see your changes.

### Contributing
- Linux
  - Install Qt 6.8.2 or higher
  - Install Qt Creator
  - Set compiler to GCC
  - If cannot find -IGL, install OpenGL development libraries
    - ex: sudo apt install libl1-mesa-dev

### Downloads
Downloads can be found in the releases section.

### Future updates
- Linux and Mac versions.
- Allow for easier saving of images (Your image is currently located within the program files as "test.jpg").
