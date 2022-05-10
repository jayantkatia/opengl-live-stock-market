#pragma once

namespace constants {
    // Stocks
    static int numStocks = 5;
    const string stockDisplayNames[] = { "NIFTY 50", "Reliance Industries", "Adani Enterprises", "Tata Consultancy Services", "HDFC Bank" };
    const string stockSymbols[] = { "^NSEI", "RELIANCE.NS", "ADANIENT.NS", "TCS.NS", "HDFCBANK.NS" };
    const string stockSymbolRequests[] = { "%5ENSEI", "RELIANCE.NS", "ADANIENT.NS", "TCS.NS", "HDFCBANK.NS" };


    // Fonts
    void* headingFont GLUT_BITMAP_HELVETICA_18;
    void* subHeadingFont = GLUT_BITMAP_HELVETICA_12;

    // Colors


    // Window Dimensions (keep at least 500px)
    int windowHeight = 500;  
    int windowWidth = 500;   
}
