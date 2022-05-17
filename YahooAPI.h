#pragma once
#define CURL_STATICLIB
#include <curl\curl.h>
#include<string>
#include<iostream>
#include<vector>
#include "picojson.h"
using namespace std;

// deserialize an array
template<typename T>
vector<T> fromJson(const picojson::value& input) {
    vector<T> values;
    if (input.is<picojson::array>())
    {
        const auto& array = input.get<picojson::array>();
        for (const auto i : array)
        {
            T value = i.get<double>();
            values.push_back(value);
        }
    }
    return values;
}

struct YahooAPIMonthData {
	vector<double> close;
	vector<unsigned int> timestamp;
    bool isProfit;
    
    YahooAPIMonthData() {};
	YahooAPIMonthData(string symbol, string json) {
        picojson::value v;
        string err = picojson::parse(v, json);
        if (!err.empty()) {
            cerr << err << endl;
        }
        v = v.get(symbol);

        this->timestamp = fromJson<unsigned int>(v.get("timestamp"));
        this->close = fromJson<double>(v.get("close"));
        if (close.size() > 0)
            isProfit = close[close.size() - 1] > close[0] ? true : false;
	}
};

class YahooAPI
{	
	string _apiKey="IVSwwCN6Lj4qaAFC2uL2M5SGIZfV0WRq2WbSDlW2";
	string performRequest(string url);
public:
    YahooAPIMonthData getMonthData(string requestSymbol, string symbol);
};

