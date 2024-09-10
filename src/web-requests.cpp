#include <curl/curl.h>
#include <stdio.h>

int main() {
  CURL *curl;
  CURLcode res;

  curl_global_init(CURL_GLOBAL_ALL);

  curl = curl_easy_init();
  if (curl) {

    curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:8080/leaderboard");
    curl_easy_setopt(curl,CURLOPT_POST,1L);
    curl_easy_setopt(curl, CURLOPT_POSTFIELDS, "id=1&score=1000");

    res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
      fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
    }

    curl_easy_cleanup(curl);
  }
  curl_global_cleanup();
}