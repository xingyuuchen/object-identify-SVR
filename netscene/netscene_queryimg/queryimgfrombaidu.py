# coding=utf-8
import requests
import base64


def getAccessToken():
    host = 'https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&' \
           'client_id=wkCVySSMMRRqhGZoKdXDBkGI&client_secret=qNhLC8UUVGbwQTlPszRSn56vbBBMEtFe'
    resp = requests.get(host)

    if resp:
        return resp.json()['access_token']
    return None


def Do():
    access_token = getAccessToken()

    request_url = "https://aip.baidubce.com/api/v1/solution/direct/imagerecognition/combination"
    f = open('/Users/cxy/Downloads/c51dfecd51b01f54399a80e05782d07a.jpeg', 'rb')
    img = base64.b64encode(f.read())

    classes = ['plant', 'animal', 'landmark']
    data = "{\"image\":\"" + str(img, 'utf-8') + "\",\"scenes\":[\"animal\",\"plant\",\"landmark\"]}"
    request_url = request_url + "?access_token=" + access_token
    headers = {'content-type': 'application/json'}
    response = requests.post(request_url, data=data, headers=headers)

    if response is None or response.json() is None or response.json()['result'] is None:
        return None

    json = response.json()['result']
    # print(json)
    target = None

    for clz in classes:
        if clz not in json:
            continue
        if 'result' not in json[clz]:
            continue
        results = json[clz]['result']
        highest = 0
        for item in results:
            if 'score' not in item or 'name' not in item:
                continue
            if item['name'][0] in ['非', '']:
                continue
            if float(item['score']) > highest:
                highest = float(item['score'])
                target = item['name']
    return target


if __name__ == '__main__':
    print(Do())
