class WR_BuildingToggleButtonComponent : SCR_ButtonBaseComponent
{
	ImageWidget m_wImage;
	TextWidget m_wText;
	Color m_cEnabledColor = Color.DarkGreen;
	Color m_cDisabledColor = Color.DarkRed;
	WR_BuildingRadialMenuComponent m_MenuComponent;
	
	override void HandlerAttached(Widget w)
	{
		m_wImage = ImageWidget.Cast(w.FindAnyWidget("Image"));
		m_wText = TextWidget.Cast(w.FindAnyWidget("Name"));
		PlayerController controller = GetGame().GetPlayerController();
		if (!controller)
			return;
		
		m_MenuComponent = WR_BuildingRadialMenuComponent.Cast( controller.FindComponent(WR_BuildingRadialMenuComponent) );
		if (!m_MenuComponent)
		{
			return;
		}
		if (m_MenuComponent.GetEnabled())
		{
			m_wImage.SetColor(m_cEnabledColor);
			m_wText.SetText("Building Mode : On");
		}
		else
		{
			m_wImage.SetColor(m_cDisabledColor);
			m_wText.SetText("Building Mode : Off");
		}
		super.HandlerAttached(w);
	}
	
	override bool OnClick(Widget w, int x, int y, int button)
	{
		super.OnClick(w, x, y, button);
		Print("Clicked");
		SetToggled(!m_bIsToggled);
		return false;
	}
	
	override bool OnMouseEnter(Widget w, int x, int y)
	{
		m_wImage.SetShadowColor(Color.FromRGBA(194, 100, 20, 255));
		super.OnMouseEnter(w, x, y);
		return false;
	}
	
	override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		m_wImage.SetShadowColor(Color.FromRGBA(0,0,0,0));
		super.OnMouseLeave(w, enterW, x, y);
		return false;
	}
	
	override void SetToggled(bool toggled, bool animate = true, bool invokeChange = true)
	{
		//super.SetToggled(toggled, animate, invokeChange);
		m_MenuComponent.SetEnabled(!m_MenuComponent.GetEnabled());
		if (m_MenuComponent.GetEnabled())
		{
			m_wImage.SetColor(m_cEnabledColor);
			m_wText.SetText("Building Mode : On");
		}
		else
		{
			m_wImage.SetColor(m_cDisabledColor);
			m_wText.SetText("Building Mode : Off");
		}
		Print("toggled: " + m_bIsToggled);
	}
}