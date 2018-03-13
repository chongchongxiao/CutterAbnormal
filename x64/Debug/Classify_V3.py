import tensorflow as tf
import os
import numpy as np


def classify():
    # 载入图片
#    if image_dir == 0:
#        print('image_dir is None')
#        return
    image_dir="E://VisualProjects//judgeImage//inception.png"
    image_data = tf.gfile.FastGFile(image_dir, 'rb').read()
    # 载入模型
    with tf.gfile.FastGFile("E://VisualProjects//judgeImage//output_graph.pb",'rb') as f:
        graph_def = tf.GraphDef()
        graph_def.ParseFromString(f.read())
        tf.import_graph_def(graph_def, name='')
    # 执行分类预测
    with tf.Session() as sess:
        softmax_tensor = sess.graph.get_tensor_by_name('final_result:0')
        predictions = sess.run(softmax_tensor, {'DecodeJpeg/contents:0': image_data})  # 图片格式是jpg格式
        predictions = np.squeeze(predictions)  # 把结果转为1维数据
        # 排序
        top_k = predictions.argsort()[::-1]
        return top_k[0]
