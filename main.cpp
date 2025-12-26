#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>


struct Pixel{
    int r, g, b;
};

char normToAscii(int normPix, const std::string& asciiSet){
    int idx = normPix * (asciiSet.size() - 1) / 255;
    return asciiSet[idx];
}

int main (int ac, char** av)
{
    // input check 
    if(ac != 2){
        std::cerr << "Error. Usage: ./ppm_to_ascii img" << std::endl;
        exit(EXIT_FAILURE);
    }
    std::string inFile = av[1];
    // input stream from  file av[1];
    std::ifstream file(inFile);
    if(!file.is_open()){
        std::cerr << "Error. Cannot open infile: " << inFile << std::endl;
        exit(EXIT_FAILURE);
    }
    // no parsing at all atm, pure hope for clean inputfile 
    std::string format;
    file >> format;
    if(format != "P3"){
        std::cerr << "Error. Currently P3 PPM format only" << std::endl;
        exit(EXIT_FAILURE);
    }
    int width, height, maxColorValue;

    file >> width >> height >> maxColorValue;

    std::vector<Pixel> pixels(width * height);

    for(int i = 0; i < width * height; i++){
        file >> pixels[i].r >> pixels[i].g >> pixels[i].b;
        //std::cout << pixels[i].r << pixels[i].g << pixels[i].b << std::endl;
    }
    file.close();
    
    std::string asciiSet = "@%#*+=-:. ";
    std::string outFile = inFile + ".replace";
    
    std::ofstream out(outFile);
    if(!out.is_open()){
        std::cerr << "Error. Cannot open outfile: " << inFile << std::endl;
        exit(EXIT_FAILURE);
    }
    // added scale to shrink the img
    int scaleX = 4;
    int scaleY = 8;
    for (int y = 0; y < height; y+=scaleY){
        for (int x = 0; x < width; x+=scaleX){
            Pixel& p = pixels[y * width + x];
            int normPix = static_cast<int>(0.299*p.r + 0.587*p.g + 0.114*p.b);
            char outChar = normToAscii(normPix, asciiSet);
            out << outChar;
        }
        out << std::endl;
    }
    out.close();
    return 0;
}