/**
 * @file main.c
 * @brief Sistema IoT Casa Inteligente
 * 
 * Conecta à rede WiFi "iot" com senha "123mudar"
 * Conecta ao broker MQTT em 192.168.0.1
 * Simula sensores de luminosidade (0-10) e temperatura (-3 a 45°C)
 * Controla luzes (GPIO 18) e ar condicionado (GPIO 19) via MQTT
 */

#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "services/mqtt_system.h"

static const char *TAG = "CASA_IOT";

void message_handler(const char* topic, const char* data, int len)
{
    ESP_LOGI(TAG, "Mensagem recebida: %s = %.*s", topic, len, data);
}

void app_main(void)
{
    ESP_LOGI(TAG, "========================================");
    ESP_LOGI(TAG, "|        Sistema IoT Casa Inteligente  |");
    ESP_LOGI(TAG, "|  WiFi: iot / Senha: 123mudar         |");
    ESP_LOGI(TAG, "|  MQTT: 192.168.0.1:1883              |");
    ESP_LOGI(TAG, "========================================");
    
    // Configurar callback para mensagens MQTT recebidas
    mqtt_set_message_callback(message_handler);
    
    // Inicializar sistema completo (WiFi + MQTT + Tasks)
    esp_err_t ret = mqtt_system_init();
    
    if (ret != ESP_OK) {
        
        ESP_LOGE(TAG, "Falha na inicialização do sistema");
        return;
    }
    
    ESP_LOGI(TAG, "Sistema inicializado com sucesso!");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "Funcionalidades ativas:");
    ESP_LOGI(TAG, "   Sensor luminosidade → casa/externo/luminosidade (0-10)");
    ESP_LOGI(TAG, "   Sensor temperatura → casa/externo/temperatura (-3 a 45°C)");
    ESP_LOGI(TAG, "   Controle luzes ← casa/externo/luminosidade (GPIO %d)", GPIO_LUZES);
    ESP_LOGI(TAG, "   Controle AR ← casa/sala/temperatura (GPIO %d)", GPIO_AR_CONDICIONADO);
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "Lógica de controle:");
    ESP_LOGI(TAG, "   Luzes: LIGA se luminosidade < 3, DESLIGA caso contrário");
    ESP_LOGI(TAG, "   AR: LIGA se temp > 23°C, DESLIGA após 10min com temp < 20°C");
    ESP_LOGI(TAG, "");
    ESP_LOGI(TAG, "Sistema rodando... Publicando sensores a cada 1 segundo");
}