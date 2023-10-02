# publisher.py
# Importe as bibliotecas ROS necessárias
import rospy
from std_msgs.msg import Int32

# Inicialize o nó
rospy.init_node("talker_node")

# Crie um objeto de publicação para publicar mensagens no tópico 'meu_topico'
talker_pub = rospy.Publisher('chat', Int32, queue_size=10)

# Crie uma mensagem do tipo MeuMsg
minha_mensagem = Int32
minha_mensagem.data = 10

# Publique a mensagem a cada 1 segundo
rate = rospy.Rate(1)  # 1 Hz
while not rospy.is_shutdown():
    talker_pub.publish(minha_mensagem)
    rate.sleep()
