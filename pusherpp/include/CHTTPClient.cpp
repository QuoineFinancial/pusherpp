#include "CHTTPClient.hpp"
#include <string>
#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <cstring> 
#include <assert.h>
#include <sstream> 

namespace Pusherpp
{
	size_t CHTTPClient::curlWrite(void *ptr, size_t size, size_t nmemb, void *stream)
	{
		std::stringstream* sstream = (std::stringstream*)(stream);
		std::string value((char*) ptr, size * nmemb);

		*sstream << value;
		
		return nmemb * size;
	}

	CHTTPClient::CHTTPClient()
	{
		
	}

	CHTTPClient::~CHTTPClient()
	{
		
	}

	std::string CHTTPClient::postRequest(const std::string& url, const std::string& message, long& httpCode) const
	{
		struct curl_slist* headers = NULL;
		std::stringstream  replyss;
		char*              buff;
		CURL*              curl;
		CURLcode           res;
		
		// Mazen: for some reason, sending message.c_str() in the post body to Pusher doesn't work..
		// while sending a copied array does :-/
		buff = new char[message.length() + 1];
		std::memcpy(buff, message.c_str(), message.length() + 1);
	
		curl = curl_easy_init();
		
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_POST, 1);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CHTTPClient::curlWrite);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&replyss);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		curl_easy_setopt(curl, CURLOPT_POSTFIELDS, buff);
		res = curl_easy_perform(curl);
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpCode);
		
		delete[] buff;
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
		
		return replyss.str();
	}

	std::string CHTTPClient::getRequest(const std::string& url, long& httpCode) const
	{
		struct curl_slist* headers = NULL;
		std::stringstream  replyss;
		CURL*              curl;
		CURLcode           res;
	
		curl = curl_easy_init();
		
		headers = curl_slist_append(headers, "Content-Type: application/json");
		curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
		curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &CHTTPClient::curlWrite);
		curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&replyss);
		curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
		res = curl_easy_perform(curl);
		curl_easy_getinfo (curl, CURLINFO_RESPONSE_CODE, &httpCode);
		
		curl_slist_free_all(headers);
		curl_easy_cleanup(curl);
		
		return replyss.str();
	}
}
