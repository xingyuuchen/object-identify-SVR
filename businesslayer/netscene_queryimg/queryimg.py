# coding=utf-8
import requests
import base64
import sys
import urllib.request
from lxml import etree


ITEM_DESC_MAX_LEN = 200


def get_access_token():
    host = 'https://aip.baidubce.com/oauth/2.0/token?grant_type=client_credentials&' \
           'client_id=wkCVySSMMRRqhGZoKdXDBkGI&client_secret=qNhLC8UUVGbwQTlPszRSn56vbBBMEtFe'
    resp = requests.get(host)

    if resp:
        return resp.json()['access_token']
    return None


def read_from_fifo():
    with open(fifo_name, 'rb') as fifo:
        byte_arr = fifo.read(int(data_len))
        return byte_arr


def do_query_img():
    access_token = get_access_token()

    request_url = "https://aip.baidubce.com/api/v1/solution/direct/imagerecognition/combination"
    img = base64.b64encode(read_from_fifo())

    classes = ['plant', 'animal', 'landmark']
    data = "{\"image\":\"" + str(img, 'utf-8') + "\",\"scenes\":[\"animal\",\"plant\",\"landmark\"]}"
    request_url = request_url + "?access_token=" + access_token
    headers = {'content-type': 'application/json'}
    response = requests.post(request_url, data=data, headers=headers)

    if response is None or response.json() is None or response.json()['result'] is None:
        return None

    json = response.json()['result']
    # print(json)
    target_name = None
    target_clz = 0

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
                target_name = item['name']
                target_clz = classes.index(clz)
    desc = 'ops'
    if target_name is not None:
        desc = get_desc(target_name)
        if desc is None or len(desc) == 0:
            desc = '暂时无法查询到' + target_name + '的详细资料'
    print("{}${}${}".format(target_clz, target_name, desc))


def get_desc(item):
    url = 'https://baike.baidu.com/item/' + urllib.parse.quote(item)
    req = urllib.request.Request(url=url, method='GET')
    response = urllib.request.urlopen(req)
    text = response.read().decode('utf-8')
    html = etree.HTML(text)
    sen_list = html.xpath('//div[contains(@class,"lemma-summary") or contains(@class,"lemmaWgt-lemmaSummary")]//text()')
    sen_list = [item.strip('\n') for item in sen_list]
    sen_list = ''.join(sen_list)

    if len(sen_list) > ITEM_DESC_MAX_LEN:
        last = sen_list.rfind('。', 0, ITEM_DESC_MAX_LEN)
        if last < 0:
            last = ITEM_DESC_MAX_LEN
    else:
        last = sen_list.rfind('。', 0, len(sen_list))
        if last < 0:
            last = len(sen_list) - 1
    return sen_list[0: last + 1]


if __name__ == '__main__':
    fifo_name = sys.argv[1]
    data_len = sys.argv[2]

    do_query_img()
    # print(get_desc('海棠'))
