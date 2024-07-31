
import requests
import json


LOGIN_URL = "https://openapi.alipan.com/oauth/authorize?client_id=c196f5667dee4124a6e66e9f8f6a214b&redirect_uri=https://www.gardilily.com/cloudland/redirect-uri.php&scope=user:base,file:all:read,file:all:write"


def main():

    url = 'https://openapi.alipan.com/adrive/v1.0/openFile/list'
    header = {
        'Content-Type': 'application/json',
        'Authorization': f'Bearer '
    }
    data = {
        'drive_id': '',
        'parent_file_id': ''
    }
    req = requests.post(url, headers=header, json=data)
    print(req.status_code)
    print(req.content)


if __name__ == '__main__':
    main()

