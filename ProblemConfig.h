#pragma once

class ProblemConfig
{
private:
    int code;      
    char category;   
    int duration;   

public:
    ProblemConfig(int c = 0, char cat = 'B', int d = 0)
    {
        code = c;
        category = cat;
        duration = d;
    }

    int getCode() const { return code; }
    char getCategory() const { return category; }
    int getDuration() const { return duration; }
};