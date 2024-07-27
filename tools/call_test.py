
import requests
import json


LOGIN_URL = "https://openapi.alipan.com/oauth/authorize?client_id=c196f5667dee4124a6e66e9f8f6a214b&redirect_uri=https://www.gardilily.com/cloudland/redirect-uri.php&scope=user:base,file:all:read,file:all:write"


def main():
    url = 'https://openapi.alipan.com/oauth/access_token'
    data = dict()
    data['client_id'] = 'c196f5667dee4124a6e66e9f8f6a214b'
    data['code'] = '74460a61a0994f9cbbda004f62bd7095'
    data['grant_type'] = 'authorization_code'
    req = requests.post(url, json=data)
    print(req.content)


if __name__ == '__main__':
    main()

