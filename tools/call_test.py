
import requests
import json


LOGIN_URL = "https://openapi.alipan.com/oauth/authorize?client_id=c196f5667dee4124a6e66e9f8f6a214b&redirect_uri=https://www.gardilily.com/cloudland/redirect-uri.php&scope=user:base,file:all:read,file:all:write"


def main():
    url = 'https://openapi.alipan.com/oauth/access_token'
    data = dict()
    data['client_id'] = 'c196f5667dee4124a6e66e9f8f6a214b'
    data['code'] = '9e04f9535dae484d83e69453a5ae2660'
    data['grant_type'] = 'authorization_code'
    data['code_verifier'] = '01234567890123456789012345678901234567890123456789'
    req = requests.post(url, json=data)
    print(req.content)


    url = 'https://openapi.alipan.com/adrive/v1.0/user/getDriveInfo'
    header = {
        'Content-Type': 'application/json',
        'Authorization': f'Bearer {req.json()["access_token"]}'
    }
    req = requests.post(url, headers=header)
    print(req.content)


if __name__ == '__main__':
    main()

