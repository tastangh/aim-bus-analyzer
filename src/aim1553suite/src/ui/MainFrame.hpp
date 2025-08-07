#pragma once // Bu başlık dosyasının projeye sadece bir kez dahil edilmesini sağlar.

#include <wx/wx.h> // Ana wxWidgets kütüphanesi. wxFrame gibi temel sınıflar için gereklidir.

// wxNotebook sınıfının tam tanımını bu başlık dosyasına dahil etmek yerine
// "forward declaration" yapıyoruz. Bu, derleme sürelerini kısaltmaya yardımcı olur,
// çünkü derleyiciye sadece 'wxNotebook' diye bir sınıfın var olduğunu ve onu
// bir işaretçi (pointer) olarak kullanacağımızı söylüyoruz.
class wxNotebook; 

// MainFrame sınıfımızı wxWidgets'ın wxFrame sınıfından türetiyoruz.
// Bu, MainFrame'in bir pencere olmasını sağlar.
class MainFrame : public wxFrame {
public:
    // Kurucu (Constructor) fonksiyon. 
    // Bu, MainFrame nesnesi oluşturulduğunda çağrılan ilk fonksiyondur.
    // Public'tir çünkü dışarıdan (app.cpp içinden) çağrılır.
    MainFrame();

private:
    // Olay Yöneticisi (Event Handler) Fonksiyonlar.
    // Kullanıcı etkileşimlerine (menüye tıklama, pencereyi kapatma) cevap verirler.
    // Private'tırlar çünkü sadece bu sınıfın içinden çağrılmaları amaçlanır.
    void onExit(wxCommandEvent& event);
    void onClose(wxCloseEvent& event);

    // Üye Değişkenler (Member Variables).
    // Sınıfın sahip olduğu verileri tutarlar.
    // m_notebook, sekmeli arayüzü tutan işaretçimizdir.
    wxNotebook* m_notebook;
};