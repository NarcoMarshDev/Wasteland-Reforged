//! Base class for any button, regardless its own content

//------------------------------------------------------------------------------------------------
modded class SCR_ButtonBaseComponent : SCR_WLibComponentBase 
{
	[Attribute("", UIWidgets.CheckBox, "Can the button be only clicked, or also toggled?")]
	bool m_bCanBeToggled;
	
	[Attribute()]
	protected bool m_bIsToggled;
	
	[Attribute("true", UIWidgets.CheckBox, "Use in built button colorization or handle it on your own")]
	bool m_bUseColorization;

	[Attribute("1 1 1 0.1", UIWidgets.ColorPicker)]
	ref Color m_BackgroundDefault;
	
	[Attribute("1 1 1 0.3", UIWidgets.ColorPicker)]
	ref Color m_BackgroundHovered;
	
	[Attribute("0.760 0.392 0.08 0.3", UIWidgets.ColorPicker)]
	ref Color m_BackgroundSelected;
	
	[Attribute("0.898 0.541 0.184 1", UIWidgets.ColorPicker)]
	ref Color m_BackgroundSelectedHovered;
	
	[Attribute("0.898 0.541 0.184 1", UIWidgets.ColorPicker)]
	ref Color m_BackgroundClicked;
	
	[Attribute("true")]
	bool m_bShowBorderOnFocus;
	
	[Attribute("false")]
	bool m_bNoBorderAnimation;
	
	[Attribute()]
	bool m_bShowBorderOnHover;

	Widget m_wBackground;
	Widget m_wBorder;
	
	// Arguments passed: SCR_ButtonBaseComponent
	ref ScriptInvoker m_OnClicked = new ref ScriptInvoker();
	// Arguments passed: SCR_ButtonBaseComponent, bool (toggled)
	ref ScriptInvoker m_OnToggled = new ref ScriptInvoker();
	
	// TODO: Rewrite - do not pass widget 
	ref ScriptInvoker m_OnFocus = new ref ScriptInvoker();
	ref ScriptInvoker<Widget> m_OnFocusLost = new ref ScriptInvoker();
		
	// Arguments passed: SCR_ButtonBaseComponent, bool border shown
	ref ScriptInvoker m_OnShowBorder = new ref ScriptInvoker();
	
	protected MenuBase m_ParentMenu; // Pointer back to menu which owns this button, initialized at FindParentMenu
	protected SCR_SubMenuBase m_ParentSubMenu;
	
	//------------------------------------------------------------------------------------------------
	override override void HandlerAttached(Widget w)
	{
		super.HandlerAttached(w);
		m_wBackground = w.FindAnyWidget("Background");
		//Print("---------------");
		//Print(w.GetName());
		//Print(m_wBackground);
		//Print("---------------");
		m_wBorder = w.FindAnyWidget("Border");
		if (m_wBorder && m_bUseColorization)
			m_wBorder.SetOpacity(0);
		
		ColorizeBackground(false);
		
		if (!m_wRoot.IsEnabled())
			OnDisabled(false);
		
		// Find parent menu. Due to init order, menu component is not attached to menu root widget right now,
		// so it must be done through call queue.
		GetGame().GetCallqueue().CallLater(FindParentMenu, 0);
	}
	
	//------------------------------------------------------------------------------------------------
	override override bool OnClick(Widget w, int x, int y, int button)
	{
		//Print("ITS CLICKING!!!!!!!!");
		super.OnClick(w, x, y, button);
		if (button != 0)
			return false;
		
		if (m_bCanBeToggled)
			SetToggled(!m_bIsToggled);
		m_OnClicked.Invoke(this);
		
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override override bool OnMouseEnter(Widget w, int x, int y)
	{
		super.OnMouseEnter(w, x, y);
				
		if (m_bMouseOverToFocus)
			return false;
		
		if (m_bShowBorderOnHover)
		{
			if (m_bNoBorderAnimation)
				m_wBorder.SetOpacity(1);
			else
				AnimateWidget.Opacity(m_wBorder, 1, m_fAnimationRate, true);
		}

		if (!m_bUseColorization)
			return false;
			
		if (!m_wBackground)
			return false;
		
		if (m_bIsToggled)
			AnimateWidget.Color(m_wBackground, m_BackgroundSelectedHovered, m_fAnimationRate);
		else
			AnimateWidget.Color(m_wBackground, m_BackgroundHovered, m_fAnimationRate);
		
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override override bool OnMouseLeave(Widget w, Widget enterW, int x, int y)
	{
		super.OnMouseLeave(w, enterW, x, y);
				
		if (m_bMouseOverToFocus)
			return false;
		
		if (m_bUseColorization && m_wBackground)
		{
			if (m_bIsToggled)
				AnimateWidget.Color(m_wBackground, m_BackgroundSelected, m_fAnimationRate);
			else
				AnimateWidget.Color(m_wBackground, m_BackgroundDefault, m_fAnimationRate);
		}
		
		if (!m_bShowBorderOnHover || (m_bShowBorderOnFocus && GetGame().GetWorkspace().GetFocusedWidget() == m_wRoot))
			return false;
		
		if (m_bNoBorderAnimation)
			m_wBorder.SetOpacity(0);
		else
			AnimateWidget.Opacity(m_wBorder, 0, m_fAnimationRate, true);
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override override bool OnFocus(Widget w, int x, int y)
	{
		super.OnFocus(w, x, y);
		if (m_bShowBorderOnFocus)
		{
			if (m_bNoBorderAnimation)
				m_wBorder.SetOpacity(1);
			else
				AnimateWidget.Opacity(m_wBorder, 1, m_fAnimationRate, true);
		}
		
		m_OnFocus.Invoke(m_wRoot);
		
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override override bool OnFocusLost(Widget w, int x, int y)
	{
		super.OnFocusLost(w, x, y);
		
		m_OnFocusLost.Invoke(w);
		
		if (!m_bShowBorderOnFocus || (m_bShowBorderOnHover && WidgetManager.GetWidgetUnderCursor() == m_wRoot))
			return false;
		
		if (m_bNoBorderAnimation)
			m_wBorder.SetOpacity(0);
		else
			AnimateWidget.Opacity(m_wBorder, 0, m_fAnimationRate, true);
		return false;
	}
	
	//------------------------------------------------------------------------------------------------
	override protected void OnMenuSelect()
	{
		Print("I PRESSED BUTTON YOOOO");
		if (!m_wRoot.IsEnabled())
			return;
		
		if (m_bCanBeToggled)
		{
			SetToggled(!m_bIsToggled);
		}
		else if (m_bUseColorization)
		{
			AnimateWidget.Color(m_wBackground, m_BackgroundClicked, m_fAnimationRate);
			GetGame().GetCallqueue().CallLater(ColorizeBackground, m_fAnimationTime * 500 + 1, false, true);
		}
		m_OnClicked.Invoke(this);
	}
	
	// Public API
	//------------------------------------------------------------------------------------------------
	override void SetToggled(bool toggled, bool animate = true, bool invokeChange = true)
	{
		if (!m_bCanBeToggled)
			return;
		
		m_bIsToggled = toggled;
		PlaySound(m_sSoundClicked);
		ColorizeBackground(animate);
		if (invokeChange)
			m_OnToggled.Invoke(this, m_bIsToggled);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool IsToggled()
	{
		return m_bIsToggled;
	}
	
	//------------------------------------------------------------------------------------------------
	override void SetToggleable(bool togglable)
	{
		m_bCanBeToggled = togglable;
	}
	
	//------------------------------------------------------------------------------------------------
	override bool GetToggleable()
	{
		return m_bCanBeToggled;
	}
	
	//------------------------------------------------------------------------------------------------
	override void ShowBorder(bool show, bool animate = true)
	{		
		if (animate)
			AnimateWidget.Opacity(m_wBorder, show, m_fAnimationRate, true);
		else if (m_wBorder)
			m_wBorder.SetOpacity(show);
		
		m_OnShowBorder.Invoke(this, show);
	}
	
	//------------------------------------------------------------------------------------------------
	override bool IsBorderShown()
	{
		if (!m_wBorder)
			return false;
		return m_wBorder.GetOpacity() != 0;
	}
	
	//------------------------------------------------------------------------------------------------
	override void ColorizeBackground(bool animate = true)
	{
		if (!m_bUseColorization || !m_wBackground)
			return;
		
		Color color;
		bool isHovered = WidgetManager.GetWidgetUnderCursor() == m_wRoot;
		if (m_bIsToggled && m_bCanBeToggled)
		{
			if (isHovered)
				color = m_BackgroundSelectedHovered;
			else
				color = m_BackgroundSelected;
		}
		else
		{
			if (isHovered)
				color = m_BackgroundHovered;
			else
				color = m_BackgroundDefault;
		}
		
		if (animate)
			AnimateWidget.Color(m_wBackground, color, m_fAnimationRate);
		else
		{
			AnimateWidget.StopAnimation(m_wBackground, WidgetAnimationColor);
			m_wBackground.SetColor(color);
		}
	}
	
	//------------------------------------------------------------------------------------------------
	override static SCR_ButtonBaseComponent GetButtonBase(string name, Widget parent, bool searchAllChildren = true)
	{
		auto comp = SCR_ButtonBaseComponent.Cast(
				SCR_WLibComponentBase.GetComponent(SCR_ButtonBaseComponent, name, parent, searchAllChildren)
			);
		return comp;
	}
	
	//------------------------------------------------------------------------------------------------
	//! Finds the parent menu of this component
	override protected void FindParentMenu()
	{
		// Find parent menu
		MenuBase parentMenu = GetGame().GetMenuManager().GetOwnerMenu(m_wRoot);
		m_ParentMenu = parentMenu;
		
		// Find parent submenu
		Widget wParent = m_wRoot.GetParent();
		while (wParent)
		{
			ScriptedWidgetEventHandler subMenuBase = wParent.FindHandler(SCR_SubMenuBase);
			if (subMenuBase)
			{
				m_ParentSubMenu = SCR_SubMenuBase.Cast(subMenuBase);
				break;
			}
			wParent = wParent.GetParent();
		}
		
		// Warning disabled since we have quite many places where a button is not in a menu
		//if (!parentMenu)
		//	Print(string.Format("Failed to find parent menu of SCR_ButtonBaseComponent %1 on widget %2", this, this.GetRootWidget().GetName()), LogLevel.WARNING);
	}
	
	//------------------------------------------------------------------------------------------------
	//! Returns true if parent menu is focused, or if there is no parent menu.
	override protected bool IsParentMenuFocused()
	{
		if (m_ParentSubMenu)
			return m_ParentSubMenu.GetShown();
		else if(m_ParentMenu)
			return m_ParentMenu.IsFocused();
		else
			return true;
	}
};