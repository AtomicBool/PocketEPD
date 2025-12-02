import qrcode
import numpy as np

def generate_qr_matrix(
        data,
        qr_border,
        qr_ec,
        qr_version,
        qr_size
    ):

    qr = qrcode.QRCode(
        border = qr_border,
        error_correction = qr_ec,
        version = qr_version
    )
    
    qr.add_data(data)
    qr.make(fit = True)

    matrix = qr.get_matrix()

    # 这里要特殊处理下
    # 用for循环重新填充下几行纯border的list [[True] for _ in range(7)]
    # 原本的代码(qrcode库的x_border = [False] * self.border) ai说会有"列表引用共享"的问题
    # 所以应该用for添加而非直接 [True] * n
    # 找了很久+问AI才找到 这就是为什么不该用tmd Python
    
    for i in range(qr_border):
        matrix[i] = [False for _ in range(len(matrix[0]))]
        matrix[len(matrix) - i - 1] = [False for _ in range(len(matrix[0]))]

    np_matrix = np.array(matrix, dtype=bool)
    if qr_size != 1:
        np_matrix = np.repeat(np_matrix, qr_size, axis=0)  # 垂直方向重复
        np_matrix = np.repeat(np_matrix, qr_size, axis=1)  # 水平方向重复
    matrix = np_matrix.tolist()
    
    # Horizontal Only Padding to Byte Alignment
    if(len(matrix[0]) % 8 != 0):
        target_width = ((len(matrix[0]) // 8) + 1) * 8
        list_to_add = [False] * (target_width - len(matrix[0]))
        # print(list_to_add)

        for i in range (len(matrix)):
            # print(f"row {i} :{len(matrix[i])} {matrix[i]}")
            matrix[i] += list_to_add

        # for i in range (len(matrix)):
            # print(f"2 - row {i} :{len(matrix[i])} {matrix[i]}")

    return matrix