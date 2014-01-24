/* (c)  oblong industries */


#include "Greenhouse.h"
#include <cmath>
#include <algorithm>

class Handler  :  public Thing { 
private:
  double max_size { -1.0 };
  bool zoomed { false };

  Node* currently_active { nullptr };
public:
  Handler ()
  { TranslationAnimateAsymp (2.0);
    SetTranslationHard (Vect { .0, .0, .0 });
  }

  void Add (const Str& nam_)
  { Str nam { nam_ }; // ug. Stupid mutable regexp.
    if (nam . Match (".*\\.[Jj][Pp][eE]?[Gg]") ||
        nam . Match (".*\\.[Pp][Nn][Gg]"))
      AddImage (nam);
    else
      AddVideo (nam);
  }

  template <typename T>
  void AdjustMaxSize (T* t)
  { if (0.0 > max_size) max_size = t -> Diagonal ();
    else max_size = std::min (max_size, t -> Diagonal ());
  }

  void AddVideo (const Str& nam)
  { Video *v = new Video (nam);
    v -> EnableLooping ();
    v -> SetVolume (0.0);
    v -> Play ();
    AppendKid (v);
    AdjustMaxSize (v);
  }

  void AddImage (const Str& nam)
  { Image *img = new Image (nam);
    AppendKid (img);
    AdjustMaxSize (img);
  }

  Vect KidLoc (const int i)
  { const int N = KidCount ();
    const double RN = std::sqrt (N);
    const double A = FeldAspect (Feld ());
    const int COLS = RN * A;

    const int x = i % COLS;
    const int y = i / COLS;

    const double Z = RN * max_size / 3.0;

    return Vect { (x - COLS / 2) * max_size, (y - (N / COLS) / 2) * max_size, - Z };
  }

  void Rejigger ()
  { const int N = KidCount ();
    for (int i = 0; i < N; ++i)
      { Node* n = NthKid (i);
        n -> SetTranslation (KidLoc (i));
        SetDiagonal (n);
      }
  }

  void SetDiagonal (Node *n)
  { if (Image* i = dynamic_cast<Image*> (n))
      i -> SetDiagonal (max_size);
    else if (Video* v = dynamic_cast<Video*> (n))
      v -> SetDiagonal (max_size);
  }

  void Activate (Node *n)
  { if (n) n -> Heartbeat ();
    if (Video *v = dynamic_cast<Video*> (n))
      { v -> Rewind ();
        v -> SetVolume (1.0);
      }
  }

  void Deactivate (Node *n)
  { if (Video *v = dynamic_cast<Video*> (n))
      { v -> SetVolume (0.0);
      }
  }

  bool HitAnAsset (PointingEvent *e)
  { const int N = KidCount ();
    for (int i = 0; i < N; ++i)
      { Node *n = NthKid (i);
        if (n -> HitCheck (e))
          { Deactivate (currently_active);
            Activate (currently_active = n);
            SetTranslation (- KidLoc (i));
            return true;
          } 
      }
    return false;
  }

  void PointingHarden (PointingEvent *e)
  { const bool hit = HitAnAsset (e);
    if (zoomed && !hit)
      { SetTranslation (Vect { .0, .0, .0 });
        zoomed = false;
        if (nullptr != currently_active)
          { Deactivate (currently_active);
            currently_active = nullptr;
          }
      }
    else zoomed = hit;
  }
};

void Setup ()
{ SetFeldsColor (Color {0.0, 0.0, 0.0, 0.0});
  Handler *h = new Handler {};
  h -> SlapOnFeld ();
  for (auto arg : args)
    h -> Add (arg);
  h -> Rejigger ();
}
