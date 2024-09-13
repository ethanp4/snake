#include <curl/curl.h>
#include <format>
#include <iostream>
#include <string>
#include "../include/web-requests.h"
#include "../include/credentials.h"

void initRequests() {
  curl_global_init(CURL_GLOBAL_ALL);
}

void cleanupRequests() {
  curl_global_cleanup();
}

void postScore(int score) {
  auto curl = curl_easy_init();
  CURLcode res;
  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/leaderboard");
    curl_easy_setopt(curl, CURLOPT_POST,1L);
    curl_easy_setopt(curl, CURLOPT_HTTPAUTH, CURLAUTH_BASIC);

    curl_easy_setopt(curl, CURLOPT_USERPWD, userpwd);

    struct curl_slist *slist1 = NULL;
    slist1 = curl_slist_append(slist1, "Content-Type: application/json");
    slist1 = curl_slist_append(slist1, "Accept: application/json");
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, slist1);

    auto json = std::format("{{\"score\": \"{}\"}}", score); 
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json.c_str());
  }

  res = curl_easy_perform(curl);
  if (res != CURLE_OK) {
    std::cout << std::format("post score failed: {}\n", curl_easy_strerror(res)) << std::endl;
  }

  curl_easy_cleanup(curl);
}

size_t writeFunction(void *ptr, size_t size, size_t nmemb, std::string* data) {
  data->append((char*) ptr, size * nmemb);
  return size * nmemb;
}

void getLeaderboard() {
  auto curl = curl_easy_init();
  CURLcode res;

  if (curl) {
    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/leaderboard");
    // curl_easy_setopt(curl, CURLOPT_NOPROGRESS, 1L);
    // curl_easy_setopt(curl, CURLOPT_TCP_KEEPALIVE, 1L);
    std::string response_string;
    std::string header_string;
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, writeFunction);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &response_string);
    curl_easy_setopt(curl, CURLOPT_HEADERDATA, &header_string);
    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      std::cout << std::format("get leaderboard failed: {}\n", curl_easy_strerror(res)) << std::endl;
    } 

    // std::cout << response_string;
    curl_easy_cleanup(curl);
  }
}

// int main() {
//   CURL *curl;
//   CURLcode res;

//   curl_global_init(CURL_GLOBAL_ALL);

//   curl = curl_easy_init();
//   if (curl) {

//     curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/leaderboard");
//     curl_easy_setopt(curl,CURLOPT_POST,1L);
//     curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "id=1&score=1000");

//     res = curl_easy_perform(curl);
//     if (res != CURLE_OK) {
//       fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
//     }

//     curl_easy_cleanup(curl);
//   }
//   curl_global_cleanup();
// }