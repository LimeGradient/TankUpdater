#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <string>
#include <string.h>
#include <cstdio>
#include <stdexcept>

#include <curl/curl.h>
#include <curl/easy.h>
#include <cpr/cpr.h>

#include "indicators.hpp"

#if defined(WIN32) || defined(_WIN32) || defined(__WIN32__) || defined(__NT__)
    #define PATH "VERSION.tanks"
#elif __APPLE__ || __linux__
    #define PATH "VERSION.tanks"
#endif

std::string latestVersion;
char *version;

bool CreateGameDir() {
    system("mkdir Game && cd Game");
    return true;
}

size_t write_data(void *ptr, size_t size, size_t nmemb, FILE *stream) {
    size_t written = fwrite(ptr, size, nmemb, stream);
    return written;
}

int download_progress_callback(void* clientp, curl_off_t dltotal,
                               curl_off_t dlnow, curl_off_t ultotal,
                               curl_off_t ulnow)
{
    indicators::ProgressBar* progress_bar =
        static_cast<indicators::ProgressBar*>(clientp);

    if (progress_bar->is_completed())
    {
        ;
    }
    else if (dltotal == 0)
    {
        progress_bar->set_progress(0);
    }
    else
    {
        int percentage =
            static_cast<float>(dlnow) / static_cast<float>(dltotal) * 100;
        progress_bar->set_progress(percentage);
    }

    // If your callback function returns CURL_PROGRESSFUNC_CONTINUE it will
    // cause libcurl to continue executing the default progress function. return
    // CURL_PROGRESSFUNC_CONTINUE;

    return 0;
}

int download_progress_default_callback(void* clientp, curl_off_t dltotal,
                                       curl_off_t dlnow, curl_off_t ultotal,
                                       curl_off_t ulnow)
{
    return CURL_PROGRESSFUNC_CONTINUE;
}

std::string extract_file_name(const std::string& url)
{
    int i = url.size();
    for (; i >= 0; i--)
    {
        if (url[i] == '/')
        {
            break;
        }
    }

    return url.substr(i + 1, url.size() - 1);
}

bool download(const std::string& url, const std::string& file_path)
{
    const std::string file_name = extract_file_name(url);

    // Hide cursor
    indicators::show_console_cursor(false);

    indicators::ProgressBar progress_bar{
        indicators::option::BarWidth{30}, indicators::option::Start{" ["},
        indicators::option::Fill{"█"}, indicators::option::Lead{"█"},
        indicators::option::Remainder{"-"}, indicators::option::End{"]"},
        indicators::option::PrefixText{file_name},
        // indicators::option::ForegroundColor{indicators::Color::yellow},
        indicators::option::ShowElapsedTime{true},
        indicators::option::ShowRemainingTime{true},
        // indicators::option::FontStyles{
        //     std::vector<indicators::FontStyle>{indicators::FontStyle::bold}}
    };

    CURL* curl;
    FILE* fp;
    CURLcode res;
    curl = curl_easy_init();
    if (curl)
    {
        fp = fopen(file_path.c_str(), "wb");
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);
        curl_easy_setopt(curl, CURLOPT_XFERINFOFUNCTION,
                         download_progress_callback);
        curl_easy_setopt(curl, CURLOPT_XFERINFODATA,
                         static_cast<void*>(&progress_bar));
        curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 0);
        // Perform a file transfer synchronously.
        res = curl_easy_perform(curl);
        curl_easy_cleanup(curl);
        fclose(fp);
    }

    // Show cursor
    indicators::show_console_cursor(true);

    if (res == CURLE_OK)
    {
        return true;
    }
    else
    {
        return false;
    }
}

void Download() {
    std::string url = "https://tank-version-manager.herokuapp.com/download.zip";
    std::string file_path = extract_file_name(url);
    if(download(url, file_path)) {
        
    }
}

void GetLatestVersion() {
	cpr::Response r = cpr::Get(cpr::Url{"https://tank-version-manager.herokuapp.com/version"});
	std::cout << r.text << std::endl;
	latestVersion = r.text;
}

bool isLatestVersion() {
	using namespace std;

	string content;
	ifstream file(PATH);
	GetLatestVersion();
	getline(file, content);
	if (content != latestVersion) {
		return false;
	}
	return true;
	file.close();
}

int main() {
	if (isLatestVersion()) {
		std::cout << "Latest Version Installed" << std::endl;
		return 0;
	} else {
		std::cout << "Needs Update" << std::endl;
		Download();
		return -1;
	}
	return -1;
}