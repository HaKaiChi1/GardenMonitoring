#include <WiFi.h>
#include <FirebaseESP32.h>
#include <time.h>

// Thông tin WiFi
const char* ssid = "Tang 4";
const char* password = "t4405406";

// Thông tin Firebase
#define FIREBASE_HOST "iotsubject-930d8-default-rtdb.firebaseio.com"
#define FIREBASE_AUTH "AIzaSyDIQpl4Gm62s5vIJYuBrYOJerswzdStV"
FirebaseData firebaseData;

// Các node cần sử dụng
const String nodes[] = {"Node1", "Node2", "Node3", "Node4"};
int currentNode = 0; // Node mặc định để gửi dữ liệu

// Các thông số giả lập (bạn có thể thay bằng cảm biến thực tế)
float temperature = 25.5;
float humidity = 60.2;
int soilMoisture = 400;

// ################################################################################
// ######################### Hàm lấy thời gian Unix epoch #########################
// ################################################################################
String getCurrentTimeEpoch() {
    time_t now = time(nullptr); // Lấy thời gian hiện tại (epoch time)
    return String(now);         // Chuyển thành chuỗi để sử dụng trong Firebase path
}

// ####################################################################
// ######################### Hàm kết nối WiFi #########################
// ####################################################################
void setup_wifi() {
    Serial.println("Đang kết nối WiFi...");
    WiFi.begin(ssid, password);
    while (WiFi.status() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nWiFi đã kết nối!");
    Serial.print("Địa chỉ IP: ");
    Serial.println(WiFi.localIP());
}

// #############################################################################
// ######################### Đồng bộ thời gian qua NTP #########################
// #############################################################################
void syncTime() {
    configTime(7 * 3600, 0, "pool.ntp.org", "time.nist.gov"); // GMT+7 (Hà Nội)
    Serial.println("Đang đồng bộ thời gian...");
    while (time(nullptr) < 100000) {
        Serial.print(".");
        delay(500);
    }
    Serial.println("\nThời gian đã đồng bộ!");
    time_t now = time(nullptr);
    Serial.println(ctime(&now));
}
// #####################################################################
// ######################### Khởi tạo Firebase #########################
// #####################################################################
void initFirebase() {
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
    Firebase.reconnectWiFi(true);
    Serial.println("Firebase đã được khởi tạo!");
}
// ################################################################################
// ######################### Hàm gửi dữ liệu lên Firebase #########################
// ################################################################################
void sendDataToFirebase(String node_id, String time_epoch, float temp, float humid, int somo) {
    String nodePath = "/" + node_id + "/" + time_epoch; // Thêm epoch time vào node path
    if (Firebase.setFloat(firebaseData, nodePath + "/temp", temp) &&
        Firebase.setFloat(firebaseData, nodePath + "/humid", humid) &&
        Firebase.setInt(firebaseData, nodePath + "/somo", somo)) {
        Serial.println("Dữ liệu đã gửi thành công!");
        Serial.println("Node: " + node_id);
        Serial.println("Thời gian (epoch): " + time_epoch);
        Serial.println("Temp: " + String(temp) + " °C");
        Serial.println("Humid: " + String(humid) + " %");
        Serial.println("Somo: " + String(somo));
    } else {
        Serial.println("Lỗi gửi dữ liệu: " + firebaseData.errorReason());
    }
}

void setup() {
    Serial.begin(115200);
    setup_wifi();
    syncTime(); // Đồng bộ thời gian
    initFirebase();
}

void loop() {
    // Cập nhật giá trị cảm biến (thay thế bằng giá trị từ cảm biến thực)
    temperature = random(20, 30) + random(0, 99) / 100.0;
    humidity = random(50, 70) + random(0, 99) / 100.0;
    soilMoisture = random(300, 500);

    // Lấy thời gian hiện tại dưới dạng Unix epoch và gửi dữ liệu lên Firebase
    String currentTime = getCurrentTimeEpoch();
    sendDataToFirebase(nodes[currentNode], currentTime, temperature, humidity, soilMoisture);

    // Chuyển qua node tiếp theo
    currentNode = (currentNode + 1) % 4; // Lặp lại 4 node: Node1 -> Node4

    // Đợi 5 giây trước khi gửi tiếp
    delay(5000);
}
