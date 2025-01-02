from PIL import Image
from PIL import GifImagePlugin,ImageSequence

# src_file = 'gif_file/1.gif'

# pil_gif = Image.open(src_file)
# gct = pil_gif.getpalette()
# i = 0
# for frame in ImageSequence.Iterator(pil_gif):
#     print(frame.info)
#     # print(frame.getpalette())
#     # print(frame.getdata())
#     # print(frame.getbbox())
#     # print(len(frame.getcolors()))
#     # print(frame.getextrema())
#     # print(frame.getpixel((0, 0)))
#     # print(frame.histogram())
#     # print(frame.mode)
#     # print(frame.size)
#     print(i,':', frame.tile, len(frame.getdata()))
#     i+=1
# # print(gct, len(gct))


def rgb888_to_rgb565( rgb, islist=False):
    if islist:
        cnt = 0
        rgb565_list = []
        while(cnt < len(rgb)):
            r,g,b = rgb[cnt:3 + cnt]
            rgb565 = (r>>3)<<11 | (g>>2)<<5 | (b>>3)
            rgb565_list += [rgb565 & 0xff, rgb565 >> 8]
            cnt += 3
        return rgb565_list
    else:
        r,g,b = rgb
        r = (r>>3) & 0xf8
        g = (g>>2) & 0xfc
        b = (b>>3) & 0xf8
        rgb565 = (r << 11) | (g << 5) | b
        return [rgb565 >> 8, rgb565 & 0xff]


# 游程编码bytes
def run_len_encode(bytes_list, sign):
    if not bytes_list:
        return bytes_list
    cnt = 1
    max_cnt = 255
    encode_list = []
    all_real_data = 0
    for i in range(1, len(bytes_list)):
        if bytes_list[i] == bytes_list[i-1]:
            if cnt == max_cnt:
                cnt = 1
            else:
                cnt += 1
            if(cnt == max_cnt):
                encode_list += [sign, bytes_list[i], cnt]
                all_real_data += cnt
        else:
            if cnt == max_cnt:
                cnt = 1
            if cnt < 4:
                encode_list += [bytes_list[i-1]] * cnt
                all_real_data += cnt
            else:
                encode_list += [sign, bytes_list[i-1], cnt]
                all_real_data += cnt
            cnt = 1
    if cnt > 0:
        encode_list += [sign, bytes_list[-1], cnt]
        all_real_data += cnt
        if all_real_data != len(bytes_list):
            print("========== run len encoded error ", all_real_data, len(bytes_list), len(encode_list))
            exit(0)
        # print("run len encoded", all_real_data, len(bytes_list), len(encode_list))
    
    return encode_list

def run_len_decode(str_list, sign):
    encoding_list = []
    i = 0
    while(i < len(str_list)):
        if str_list[i] == sign:
            encoding_list += [str_list[i+1]] * str_list[i+2]
            i += 3
        else:
            encoding_list.append(str_list[i])
            i += 1
    return encoding_list




def om_comparess_string(bytes_list):
    a = list(bytes_list)
    for i in range(257):
        if i not in a:
            break
    if i == 257:
        print("====================error")
        exit(0)
        return (0, _, bytes_list)
    sign = i                    # 这一帧中，i是没有出现的，所以可以用i作为标志位
    result = run_len_encode(bytes_list, sign)
    dec_re = run_len_decode(result, sign)
    # ("run len decode", len(dec_re))
    return (1, sign, 1, result)


def om_gen_gif_c(dst_c_file, src_gif_file):
    pil_gif = Image.open(src_gif_file)
    
    c_file_path = dst_c_file
    c_file = open(c_file_path, 'w')
    c_file_write_raw_bytes = ''
    c_file_write_raw_num = 0
    c_file_wtite_content_end = '\n};'
    pil_gif = Image.open(src_gif_file)
    gct = pil_gif.getpalette()                                  # 获取全局调色板
    width, height = pil_gif.size
    background = pil_gif.info['background']
    # ("===========================",background)
    type = 1
    
    ''' 这7个字节,是header信息'''
    c_file_write_raw_bytes += '0x{:02x}, 0x{:02x}, '.format(width & 0xff, width >> 8)
    c_file_write_raw_bytes += '0x{:02x}, 0x{:02x}, '.format(height & 0xff, height >> 8)
    c_file_write_raw_bytes += '0x{:02x}, '.format(type)
    c_file_write_raw_bytes += '0x{:02x}, '.format(len(gct)//3 - 1)
    c_file_write_raw_bytes += '0x{:02x}, '.format(background)
    c_file_write_raw_num += 7
    c_file_write_raw_bytes += '\n\n\n\n'
    
    ''' 这是全局调色板 '''
    gct565_byte_list = rgb888_to_rgb565(gct, islist = True)
    for w_byte in gct565_byte_list:
        c_file_write_raw_bytes += '0x{:02x},'.format(w_byte)
    c_file_write_raw_bytes += '\n\n\n\n'
    c_file_write_raw_num += len(gct565_byte_list)

    # 保存所有帧的数据
    for frame in ImageSequence.Iterator(pil_gif):
        if frame.tile != []:
            all_w_h_t_l = list(frame.tile[0][1]) + [0, 0]
        else:
            all_w_h_t_l = [0, 0,width,height, 0 , 0]
        # print(all_w_h_t_l,frame.info)
        
        x1 = all_w_h_t_l[0]
        y1 = all_w_h_t_l[1]
        x2 = all_w_h_t_l[2]
        y2 = all_w_h_t_l[3]
        frame_data = list(frame.getdata())
        sub_frame_data = []
        for y in range(y1, y2):
            sub_frame_data += frame_data[y * width + x1:y * width + x2]
        
        # (len(frame_data), len(sub_frame_data), x1, y1, x2, y2)


        frame_sub_np_bytes = sub_frame_data
        
        w = x2 - x1
        h = y2 - y1
        l = x1
        t = y1
        delay = all_w_h_t_l[4]
        dispose = all_w_h_t_l[5]
        # frame_sub = pil_gif.crop((l, t, l+w, t+h))            # 按照frame的真实大小裁剪, 此时保存的是索引
        # frame_sub_np = np.array(frame_sub)
        # frame_sub_np_bytes = frame_sub_np.tobytes()
        # 调色板
        lct = pil_gif.getpalette()
        if lct != gct:
            print(lct)

        ''' 14字节的帧头信息 '''
        c_file_write_raw_bytes += '0x{:02x}, 0x{:02x}, '.format(w & 0xff, w >> 8)
        c_file_write_raw_bytes += '0x{:02x}, 0x{:02x}, '.format(h & 0xff, h >> 8)
        c_file_write_raw_bytes += '0x{:02x}, 0x{:02x}, '.format(l & 0xff, l >> 8)
        c_file_write_raw_bytes += '0x{:02x}, 0x{:02x}, '.format(t & 0xff, t >> 8)
        c_file_write_raw_bytes += '0x{:02x}, 0x{:02x}, '.format(delay & 0xff, delay >> 8)
        c_file_write_raw_bytes += '0x{:02x}, '.format(dispose)
        c_file_write_raw_num += 11

        # 每帧图片的索引数据
        w_cnt = 0
        if 1:   # 1表示压缩，0表示不压缩
            flag, sign, sign_bytes, str_dec = om_comparess_string(frame_sub_np_bytes)
            c_file_write_raw_bytes += '0x{:02x}, '.format(flag)
            c_file_write_raw_bytes += '0x{:02x}, '.format(sign)
            c_file_write_raw_bytes += '0x{:02x}, '.format(sign_bytes)
            w_cnt += 3
        else:
            str_dec = frame_sub_np_bytes
            c_file_write_raw_bytes += '0x{:02x}, '.format(0)
            c_file_write_raw_bytes += '0x{:02x}, '.format(0)
            c_file_write_raw_bytes += '0x{:02x}, '.format(0)
            w_cnt += 3
        c_file_write_raw_bytes += '\n\n\n'
        
        for single_byte in str_dec:
            if w_cnt % 32 == 0:
                c_file_write_raw_bytes += '\n'
            c_file_write_raw_bytes += '0x{:02x},'.format(int(single_byte))
            w_cnt += 1
        #(w_cnt)
        c_file_write_raw_bytes += '\n\n\n'
        c_file_write_raw_num += w_cnt

    c_file_wtite_content_header = 'const unsigned char om_gif_{} [{}] = '.format(src_gif_file.split('.')[0], c_file_write_raw_num) + '{\n'
    c_file_write_all = c_file_wtite_content_header + c_file_write_raw_bytes + c_file_wtite_content_end
    c_file.write(c_file_write_all)
    c_file.close()
if __name__ == "__main__":
    import sys
    dst_c_file = sys.argv[1]
    src_gif_file = sys.argv[2]
    om_gen_gif_c(dst_c_file, src_gif_file)