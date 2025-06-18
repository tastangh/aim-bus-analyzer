/* SPDX-FileCopyrightText: 2024 AIM GmbH <info@aim-online.com> */
/* SPDX-License-Identifier: MIT */

#ifndef BC_HPP
#define BC_HPP

// --- Gerekli temel AIM API başlık dosyaları ---
// mil_util.h bağımlılığı tamamen kaldırılmıştır.
#include "AiOs.h"
#include "Api1553.h"

/**
 * @class BusController
 * @brief MIL-STD-1553 Bus Controller (BC) işlemlerini yöneten, bağımsız bir C++ sınıfı.
 *
 * Bu sınıf, harici bir yardımcı kütüphane (mil_util) gerektirmeden, yalnızca
 * temel AIM API'sini kullanarak bir AIM kartını BC olarak yönetir.
 */
class BusController
{
public:
    /**
     * @brief BusController sınıfının kurucusu.
     * @param device Kullanılacak kartın modül numarası.
     * @param stream Kullanılacak akış (stream) numarası.
     */
    BusController(AiUInt32 device, AiUInt32 stream);

    /**
     * @brief BusController sınıfının yıkıcısı.
     * Kaynakları otomatik olarak temizler (ApiClose, ApiExit).
     */
    ~BusController();

    /**
     * @brief Kartı ve API'yi başlatır.
     * @return Başlatma başarılı olursa true, aksi takdirde false döner.
     */
    bool initialize();

    /**
     * @brief BC örnek simülasyonunu çalıştırır.
     */
    void runSample();

    /**
     * @brief API bağlantısını kapatır ve kaynakları serbest bırakır.
     */
    void shutdown();

private:
    /**
     * @brief Simülasyon için BC transferlerini ve çerçevelerini hazırlar.
     * @return Hazırlık başarılı olursa API_OK, aksi takdirde bir hata kodu döner.
     */
    AiReturn prepare();

    /**
     * @brief Tek bir BC transfer tanımı oluşturur.
     * MilUtilBCCreateTransfer fonksiyonunun yerini alır.
     * @param transferId Transfer için benzersiz kimlik.
     * @param type Transfer tipi (örn. API_BC_TYPE_BCRT).
     * @param txRt, txSa, rxRt, rxSa, wordCount Transfer parametreleri.
     * @param headerId Bu transfer için kullanılacak başlık kimliği.
     * @return Başarılı olursa API_OK, aksi takdirde hata kodu döner.
     */
    AiReturn createBcTransfer(AiUInt16 transferId, AiUInt8 type,
                              AiUInt8 txRt, AiUInt8 txSa,
                              AiUInt8 rxRt, AiUInt8 rxSa,
                              AiUInt8 wordCount, AiUInt16 headerId);
    
    // Sınıf üyeleri
    AiUInt32 m_device;          ///< Kart modül numarası
    AiUInt32 m_stream;          ///< Kart akış numarası
    AiUInt32 m_modHandle;       ///< API tarafından döndürülen modül tanıtıcısı
    bool m_isInitialized;       ///< initialize() fonksiyonunun başarıyla çağrılıp çağrılmadığı

    // mil_util'deki global state'in yerini alan özel üyeler
    AiUInt32 m_nextBcHeaderId;  ///< Bir sonraki BC başlık kimliği için sayaç
    AiUInt32 m_nextBufferId;    ///< Bir sonraki buffer kimliği için sayaç
};

#endif // BC_HPP